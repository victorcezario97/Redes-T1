#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <server_client.h>

#define CONMSG1 "Conectado! Aguardando outro jogador...\n"
#define CONMSG2 "Conectado! Começando jogo...\n"

int main(int argc, char *argv[]){

	int sockfd, cli1_sockfd, cli2_sockfd;
	struct sockaddr_in serv_addr, cli_addr1, cli_addr2;
	socklen_t clilen1, clilen2;
	int port;

	if(argc < 2){
		printf("usage %s: port\n", argv[0]);
		return -1;
	}

	port = atoi(argv[1]);

	//Starting the server
	startServer(&sockfd, &serv_addr, (unsigned short)port);

	//Listening on the created socket
	listen(sockfd, 1);

	//Accepting the 2 clients
	acceptClient(&cli_addr1, &clilen1, sockfd, &cli1_sockfd, CONMSG1);
	acceptClient(&cli_addr2, &clilen2, sockfd, &cli2_sockfd, CONMSG2);


	return 0;
}
