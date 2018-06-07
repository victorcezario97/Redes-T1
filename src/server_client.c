#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>

#define CONMSG1 "Conectado! Aguardando outro jogador...\n"
#define CONMSG2 "Conectado! Começando jogo...\n"

int startServer(int *sockfd, struct sockaddr_in *serv_addr, unsigned short port){
	int er;

	(*sockfd) = socket(AF_INET, SOCK_STREAM, 0);
	if((*sockfd) < 0){
		printf("ERROR SOCKET\n");
		return -1;
	}

	bzero((char *) serv_addr, sizeof(*serv_addr));

	(*serv_addr).sin_family = AF_INET;
	(*serv_addr).sin_addr.s_addr = INADDR_ANY;
	(*serv_addr).sin_port = htons(port);
	er = bind((*sockfd), (struct sockaddr *) serv_addr, sizeof(*serv_addr));
	if(er < 0){
		printf("ERROR BIND\n");
		return -1;
	}

	return 0;
}

int acceptClient(struct sockaddr_in *cli_addr, socklen_t *len, int sockfd, int *cli_sockfd, char *msg){

	int er;

	(*len) = sizeof((*cli_addr));
	printf("Waiting for client...\n");
	(*cli_sockfd) = accept(sockfd, (struct sockaddr *) cli_addr, len);
	if((*cli_sockfd) < 0){
		printf("ERROR CLIENT 1\n");
		return -1;
	}else printf("Cliente conectado.\n");

	er = write((*cli_sockfd), msg, strlen(msg));
	if(er < 0) printf("ERROR WRITE\n");

	return 0;
}

int startClient(int *sockfd, struct hostent *server, struct sockaddr_in *serv_addr, unsigned short port, char *hostname){
	int er;

	(*sockfd) = socket(AF_INET, SOCK_STREAM, 0);
	if((*sockfd) < 0){
		printf("ERROR SOCKET\n");
		return -1;
	}

	server = gethostbyname(hostname);
	if(server == NULL){
		printf("ERROR HOST\n");
		return -1;
	}

	bzero((char *) serv_addr, sizeof((*serv_addr)));
	(*serv_addr).sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *)&(*serv_addr).sin_addr.s_addr, server->h_length);
	(*serv_addr).sin_port = htons(port);

	er = connect((*sockfd),(struct sockaddr *) serv_addr, sizeof((*serv_addr)));
	if(er < 0){
		printf("ERROR CONNECT: %d\n", errno);
		return -1;
	}

	return 0;
}

