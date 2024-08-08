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
#include <signal.h>

enum {
    P_TCP,
    P_UDP,
};

#define serv(port, o...) \
    ({ int backlog = 10, max_buf = 65507,\
        protocol = P_TCP; o;\
    serv_(port, backlog, max_buf, protocol); })

#define con(host, port, o...) \
    ({ int max_buf = 65507, protocol = P_TCP; o;\
    con_(host, port, max_buf, protocol); })

int serv_(int port, int backlog, int max_buf, int protocol);
int con_(char* host, int port, int max_buf, int protocol);
int end(int socket);

#endif