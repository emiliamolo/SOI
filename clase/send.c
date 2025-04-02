#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define MAXMSG 255
#define SOCK_PATH "/home/alumno/socket"

int main() {
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_PATH);

    socklen_t addrsz = sizeof(addr);

    char buf[MAXMSG+1];
    int running = 1;
    while (running && scanf("%[^\n]",buf) != -1) {
        if (!strcmp(buf, "CHAU")) 
            running = 0;
        sendto(fd, buf, MAXMSG, 0, (struct sockaddr*)&addr, addrsz);
        getchar();
        memset(buf, 0, sizeof(buf));
    }


    return 0;
}