#include "board.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <arpa/inet.h>
//#include <sys/types.h>
//#include <netinet/in.h>
using namespace std;
#define PORT "5500"
#define BACKLOG 10

class SServer {
	public:
		void init();
		void round();
		bool waitplayers();
		void waitplayerid(int id);
		void syncboard(int id);
		void sigchld_handler(int s);
		void *get_in_addr(struct sockaddr *sa);

	private:
		squares board;
		int playernum;
		int lastwin;
		bool firstround;
		set<int> players;

		int sockfd, newsockfd;
		struct addrinfo hints, *servinfo, *p;
		struct sockaddr_storage addrs;
		struct sigaction sa;
		struct sockaddr_storage cli_addr;
		char s[INET6_ADDRSTRLEN];

};
