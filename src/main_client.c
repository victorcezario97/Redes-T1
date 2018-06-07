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

int main(int argc, char *argv[])
{
	int sockfd, port, er;
	struct sockaddr_in serv_addr;
	struct hostent *server = NULL;
	char buffer[256], *hostname;

	if(argc < 3){
		printf("usage %s: port hostname\n", argv[0]);
		return -1;
	}

	port = atoi(argv[1]);
	hostname = (char*)malloc(sizeof(argv[2])*sizeof(char));
	strcpy(hostname, argv[2]);

	startClient(&sockfd, server, &serv_addr, (unsigned short)port, hostname);

	er = read(sockfd, &buffer, sizeof(buffer));
	if(er < 0){
		printf("ERROR READ\n");
		return -1;
	}
	
	printf("%s\n", buffer);

	return 0;
}