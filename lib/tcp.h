#ifndef __NET_H
#define __NET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

#define serv(o...) \
    ({ int port = 3000, backlog = 10; o; serv_(port, backlog); })

int serv_(int, int);
int con(char*, int);
int end(int);

#endif