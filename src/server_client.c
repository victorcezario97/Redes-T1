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

//Sets up the server
int startServer(int *sockfd, struct sockaddr_in *serv_addr, unsigned short port){
	int er;

	//Creating the socket
	(*sockfd) = socket(AF_INET, SOCK_STREAM, 0);
	//Checking for errors
	if((*sockfd) < 0){
		printf("ERROR SOCKET\n");
		return -1;
	}

	//Clearing the server address
	bzero((char *) serv_addr, sizeof(*serv_addr));

	//Setting the server address properties
	(*serv_addr).sin_family = AF_INET;
	(*serv_addr).sin_addr.s_addr = INADDR_ANY;
	(*serv_addr).sin_port = htons(port);

	//Binding the server address to the socket
	er = bind((*sockfd), (struct sockaddr *) serv_addr, sizeof(*serv_addr));
	if(er < 0){
		printf("ERROR BIND\n");
		return -1;
	}

	return 0;
}

//Waits for a client to connect
int acceptClient(struct sockaddr_in *cli_addr, socklen_t *len, int sockfd, int *cli_sockfd, char *msg){

	int er;

	(*len) = sizeof((*cli_addr));

	printf("Waiting for client...\n");
	//Ready to accept the connection from the client
	(*cli_sockfd) = accept(sockfd, (struct sockaddr *) cli_addr, len);
	if((*cli_sockfd) < 0){
		printf("ERROR CLIENT 1\n");
		return -1;
	}else printf("Cliente conectado.\n");

	//Sending a confirmation message to the client
	er = write((*cli_sockfd), msg, strlen(msg));
	if(er < 0) printf("ERROR WRITE\n");

	return 0;
}

//Sets up the client and connects to the server
int startClient(int *sockfd, struct hostent *server, struct sockaddr_in *serv_addr, unsigned short port, char *hostname){
	int er;

	//Creating the socket
	(*sockfd) = socket(AF_INET, SOCK_STREAM, 0);
	if((*sockfd) < 0){
		printf("ERROR SOCKET\n");
		return -1;
	}

	//Getting the specified host
	server = gethostbyname(hostname);
	if(server == NULL){
		printf("ERROR HOST\n");
		return -1;
	}

	//Clearing the server address
	bzero((char *) serv_addr, sizeof((*serv_addr)));
	(*serv_addr).sin_family = AF_INET;
	//Copying the address and the port
	bcopy((char *) server->h_addr, (char *)&(*serv_addr).sin_addr.s_addr, server->h_length);
	(*serv_addr).sin_port = htons(port);

	//Attempting to connect to the server
	er = connect((*sockfd),(struct sockaddr *) serv_addr, sizeof((*serv_addr)));
	if(er < 0){
		printf("ERROR CONNECT: %d\n", errno);
		return -1;
	}

	return 0;
}
/*
void readMessage(int sockfd, char buffer[500]){
	//Reading a message.
	int er = read(sockfd, &buffer, sizeof(buffer));
	if(er < 0){
		printf("ERROR READ\n");
		return -1;
	}
	//Prints message
	printf("%s\n", buffer);
}

void writeMessage(int sockfd, char buffer[500]){

}*/