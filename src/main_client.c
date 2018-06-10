#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <server_client.h>

#define bool int
#define true 1
#define false 0

int main(int argc, char *argv[])
{
	int sockfd, port, er;
	struct sockaddr_in serv_addr;
	struct hostent *server = NULL;
	char buffer[300], *hostname, input[10], msgCompare[40], msgCompare2[40], msgCompare3[40];
	bool positionsDone = false, gameOver = false;
	

	if(argc < 3){
		printf("usage %s: port hostname\n", argv[0]);
		return -1;
	}

	//Getting the arguments
	port = atoi(argv[1]);
	hostname = (char*)malloc(sizeof(argv[2])*sizeof(char));
	strcpy(hostname, argv[2]);

	//Starting the client
	startClient(&sockfd, server, &serv_addr, (unsigned short)port, hostname);

	while(positionsDone == false){
		//Reading a message from the server
		er = read(sockfd, &buffer, sizeof(buffer));
		if(er < 0){
			printf("ERROR READ\n");
			return -1;
		}

		strcpy(msgCompare, "Done");
		// Compare
		if(strcmp(buffer, msgCompare) == 0)
			positionsDone = true;
		else{	// Input
			scanf("%s", input);
			er = write((sockfd), input, strlen(input));
			if(er < 0) printf("ERROR WRITE\n");
		}
	}
	
	while(gameOver == false){
		//Reading a message from the server
		er = read(sockfd, &buffer, sizeof(buffer));
		if(er < 0){
			printf("ERROR READ\n");
			return -1;
		}

		strcpy(msgCompare, "Aguarde o adversario\n");
		// Player turn
		if(strcmp(buffer, msgCompare) != 0){
			strcpy(msgCompare, "Parabens, player 1!\n");
			strcpy(msgCompare2, "Parabens, player 2!\n");
			strcpy(msgCompare3, "Voce perdeu!\n");

			if(strcmp(buffer, msgCompare) == 0 || strcmp(buffer, msgCompare2) == 0 || 
				strcmp(buffer, msgCompare3) == 0)
				gameOver = true;
			else{
				scanf("%s", input);
				er = write((sockfd), input, strlen(input));
				if(er < 0) printf("ERROR WRITE\n");
			}
		}
	}

	//printf("%s\n", buffer);

	return 0;
}