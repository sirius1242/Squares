#include "server.hpp"

void SServer::init()
{
	board.init();
	playernum = 0;
	lastwin = 0;
	firstround = true;
	players = {0, 1, 2, 3};
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int rv;
	int yes;

	if((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
	{
		cerr << "getaddrinfo: " << gai_strerror(rv) << endl;
		return;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p!= NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == 1)
		{
			perror("setopt");
			return;
		}

		if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if(p == NULL)
	{
		cerr << "server: failed to bind" << endl;
		return;
	}

	if(listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		return ;
	}

	sa.sa_handler = sigchld_handler; //reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if(sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		perror("sigaction");
		return ;
	}

	cout << "server: waiting for connections..." << endl;

	waitplayers();

}

void SServer::sigchld_handler(int s)
{
	int save_errno = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = save_errno;
}

void *SServer::get_in_addr(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void SServer::round() // deal with every game round
{
	for(int i=0; i<playernum; i++)
	{
		int playerid = players[(lastwin+i)%players.size()];
		do {
			waitplayerid(playerid);
		} while(!board.tryinsert(, , , playerid, firstround));
		board.insert(, , , playerid, firstround);
	}

	vector<int> loseplayers;
	if(board.check(loseplayers))
	{
		for(auto &i: loseplayers)
			players.erase(i);
	}
	if(firstround)
		firstround = false;
}

bool SServer::waitplayers()
{
	for(int i=0; i<4; i++)
	{
		int sin_size = sizeof cli_addr;
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &sin_size);
		if(newsockfd = -1)
		{
			perror("accept");
			continue;
		}

		inet_ntop(cli_addr.ss_family, get_in_addr((struct sockaddr *)&cli_addr), s, sizeof s);
		cout << "server: got connection from " << s << endl;

	}

}

int main()
{

}
