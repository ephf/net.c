#include "tcp.h"

int serv_(int p, int bl) {
    int s = socket(PF_INET, SOCK_STREAM, 0), r = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &r, sizeof(r));
    if(s == -1 || bind(s, (void*) &(struct sockaddr_in) {
        .sin_family = AF_INET, .sin_port = htons(p) },
        sizeof(struct sockaddr_in))
    || listen(s, bl)) {
        perror("serv()"); return 0; }
    while((r = accept(s, 0, 0)) && fork());
    return r;
}

int con(char* a, int p) {
    int s = socket(PF_INET, SOCK_STREAM, 0);
    struct addrinfo* i;
    if(s == -1 || getaddrinfo(a, 0,
        &(struct addrinfo) { .ai_family = PF_INET,
        .ai_socktype = SOCK_STREAM }, &i)
    || connect(s, (void*) &(struct sockaddr_in) {
        .sin_family = AF_INET, .sin_port = htons(p),
        .sin_addr = ((struct sockaddr_in*)(void*)
            i->ai_addr)->sin_addr },
        sizeof(struct sockaddr_in))) {
        perror("con()"); return 0; }
    return s;
}

int end(int s) {
    return shutdown(s, SHUT_RDWR) ?: close(s);
}