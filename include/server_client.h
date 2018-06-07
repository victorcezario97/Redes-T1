#ifndef _SERVER_H_
#define _SERVER_H_

#include <netdb.h>

int startServer(int *, struct sockaddr_in *, unsigned short);
int acceptClient(struct sockaddr_in *, socklen_t *, int, int *, char *);
int startClient(int *sockfd, struct hostent *server, struct sockaddr_in *serv_addr, char *port, char *hostname);

#endif
