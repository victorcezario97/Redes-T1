#ifndef _SERVER_H_
#define _SERVER_H_

int startServer(int *, struct sockaddr_in *, unsigned short);
int acceptClient(struct sockaddr_in *, socklen_t *, int, int *, char *);

#endif
