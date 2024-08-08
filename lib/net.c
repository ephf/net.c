#include "net.h"

int serv_(int port, int backlog, int max_buf, int protocol) {
    signal(SIGCHLD, SIG_IGN);

    int serv = socket(PF_INET, protocol == P_TCP
        ? SOCK_STREAM : SOCK_DGRAM, 0), req[2] = { 1 }, n;
    
    if(serv == -1
    || protocol == P_TCP &&
    setsockopt(serv, SOL_SOCKET, SO_REUSEADDR, req,
        sizeof(*req))
    || bind(serv, (void*) &(struct sockaddr_in) {
        .sin_family = AF_INET, .sin_port = htons(port) },
        sizeof(struct sockaddr_in))
    || protocol == P_TCP &&
        listen(serv, backlog)) {
            perror("serv()"); return -1; }
    
    if(protocol == P_TCP) {
        while((*req = accept(serv, 0, 0)) && fork())
            ;;
        return *req;
    }

    printf("SERVER UDP\n");

    struct sockaddr_in cl;
    char buf[max_buf];
    while((n = recvfrom(serv, buf, max_buf, 0,
        (void*) &cl, &(int) { sizeof(cl) })) && fork())
        ;;
    
    socketpair(AF_UNIX, SOCK_STREAM, 0, req);
    write(req[1], buf, n);
    if(fork()) { while((n = read(req[1], buf, max_buf)) > 0)
        sendto(serv, buf, n, 0, (void*) &cl, sizeof(cl));
        end(req[1]); exit(0); }
    return *req;
}

int con_(char* host, int port, int max_buf, int protocol) {
    signal(SIGCHLD, SIG_IGN);

    int con = socket(PF_INET, protocol == P_TCP
        ? SOCK_STREAM : SOCK_DGRAM, 0), req[2], n;
    struct addrinfo* ai;
    
    if(con == -1
    || getaddrinfo(host, 0, 0, &ai)
    || connect(con, (void*) &(struct sockaddr_in) {
        .sin_family = AF_INET, .sin_port = htons(port),
        .sin_addr = ((struct sockaddr_in*)(void*)
            ai->ai_addr)->sin_addr },
        sizeof(struct sockaddr_in))) {
            perror("con()"); return -1; }
    
    if(protocol == P_TCP)
        return con;
    
    socketpair(AF_UNIX, SOCK_STREAM, 0, req);
    char buf[max_buf];

    if(fork()) { while((n = read(req[1], buf, max_buf)) > 0)
        sendto(con, buf, n, 0, 0, 0);
        end(con); exit(0); }
    if(fork()) { while((n = recvfrom(con, buf, max_buf,
        0, 0, 0)) > 0) write(req[1], buf, n);
        exit(0); }
    
    return *req;
}

int end(int socket) {
    return shutdown(socket, SHUT_RDWR) ?: close(socket);
}