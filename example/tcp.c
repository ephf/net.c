#include <net.h>

int main() {
    if(fork()) {
        // server
        int req = serv(3000); // (backlog=10)

        char buf[16];
        read(req, buf, 16);
        printf("[recv] %.*s\n", 16, buf);

        return end(req);
    }

    // client
    for(int i = 0; i < 10; i++) {
        int req = con("localhost", 3000);
        dprintf(req, "Hello World (%d)", i);
        end(req);
    }
}