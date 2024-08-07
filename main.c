#include "lib/tcp.h"

int main() {
    if(fork()) {
        // server
        int req = serv(port=3001, backlog=10);

        char buf[16];
        read(req, buf, 16);
        printf("[recv] %.*s\n", 16, buf);

        return end(req);
    }

    // client
    int req = con("localhost", 3001);
    dprintf(req, "Hello World");
    end(req);
}