# c TCP/UDP

Simple tcp and udp client/server in c

> tcp example (see [tcp.c](./example/tcp.c)) `$ make tar=tcp`

```c
#include "lib/net.h"

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
```

> udp example (see [udp.c](./example/udp.c)) `$ make tar=udp`

*udp is changed to conform to the basic file descriptor io and not `sendto` and `recvfrom`*

```c
#include "lib/net.h"

int main() {
    if(fork()) {
        // server
        int req = serv(3000, protocol=P_UDP); // (max_buf=65507)

        char buf[16];
        read(req, buf, 16);
        printf("[recv] %.*s\n", 16, buf);

        return end(req);
    }

    // client
    for(int i = 0; i < 10; i++) {
        int req = con("localhost", 3000, protocol=P_UDP); // (max_buf=65507)
        dprintf(req, "Hello World (%d)", i);
        end(req);
    }
}
```