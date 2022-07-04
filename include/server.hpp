#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "board.hpp"
//#include <sys/types.h>
//#include <netinet/in.h>
using namespace std;
#define PORT "5500"
#define BACKLOG 10

class SServer
{
   public:
    void init();
    SServer();
    void begin();
    bool round(bool firstround);
    bool waitplayers();
    void waitplayerid(int id, int (&insert_args)[4]);
    void broadcast_move(int insert_args[4]);
    void syncboard(int id);
    void *get_in_addr(struct sockaddr *sa);
    bool after_move(bool firstround);
    void end_game();

   private:
    Squares board;
    int lastwin;
    int active_player;
    set<int> lostplayers;

    int sockfd, newsockfd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage addrs;
    struct sigaction sa;
    struct sockaddr_storage cli_addrs[PNUM];
    char s[INET6_ADDRSTRLEN];
};
