#include "server.hpp"

void sigchld_handler(int s)
{
    int save_errno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
    errno = save_errno;
}

void SServer::init()
{
    board.init();
    active_player = 0;
    lastwin = 0;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
}

bool SServer::begin() {
    int yes = 1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        return false;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof(int)) == 1)
    {
        perror("setopt");
        close(sockfd);
        return false;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
    };
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("server: bind");
        close(sockfd);
        return false;
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        close(sockfd);
        return false;
    }

    if (p == NULL)
    {
        cerr << "server: failed to bind" << endl;
        return false;
    }

    return true;
}

void SServer::run_loop()
{
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        return;
    }

    begin();
    waitplayers();
    round(true);
    while (round(false))
        ;
    end_game();
}

SServer::SServer()
{
    init();
}

void *SServer::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

bool SServer::after_move(bool firstround) // judge whether there are players lost
{
    if (!firstround)
    {
        if (int loseplayers = board.check())
        {
            int i = 0;
            while (loseplayers)
            {
                if (loseplayers % 2 == 1)
                {
                    lostplayers.insert(i);
                    if (lostplayers.size() == PNUM)
                        std::cout << "Player " << i << " wins!" << std::endl;
                    else
                        std::cout << "Player " << i << " loses!" << std::endl;
                }
                loseplayers >>= 1;
                i++;
            }
        }
    }
    if (!lostplayers.empty())
    {
        if (lostplayers.size() == PNUM)
        {
            active_player = -1;
            return false;
        }
    }
    return true;
}

bool SServer::round(bool firstround) // deal with every game round
{
    for (int i = 0; i < PNUM; i++)
    {
        int insert_args[4];
        active_player = lastwin + i % PNUM;
        if (!lostplayers.empty())
        {
            while (lostplayers.count(active_player))
            {
                active_player++;
                active_player %= PNUM;
            }
        }
        do
        {
            waitplayerid(active_player, insert_args);
        } while (!board.tryinsert(insert_args[0], insert_args[1], std::make_pair(insert_args[2], insert_args[3]), active_player, firstround));
        board.insert(insert_args[0], insert_args[1], std::make_pair(insert_args[2], insert_args[3]), active_player, firstround);
        broadcast_move(insert_args);
        if (!after_move(firstround)) // game end
            return false;
    }
    return true;
}

bool SServer::waitplayers() // wait for all players establish the connect
{
    cout << "server: waiting for connections..." << endl;
    for (int i = 0; i < PNUM; i++)
    {
        struct sockaddr_storage cli_addr;
        socklen_t sin_size = sizeof cli_addr;
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &sin_size);
        if (newsockfd == -1)
        {
            perror("accept");
            continue;
        }
        cli_addrs[i] = cli_addr;

        inet_ntop(cli_addr.ss_family, get_in_addr((struct sockaddr *)&cli_addr), s, sizeof s);
        cout << "server: got connection from " << s << endl;
    }

    return true;
}

void SServer::end_game() // end game and reset server
{
}

void SServer::waitplayerid(int id, int (&insert_args)[4]) // wait for active player to move
{
}

void SServer::broadcast_move(int insert_args[4]) // broadcast the move of active player to every one
{
}

int main()
{
    SServer server;
    while (true)
        server.run_loop();
}
