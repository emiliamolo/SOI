#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define MAXMSG 255
#define SOCK_PATH "/home/alumno/socket"

int fd;

void close_srv() {
    close(fd);
    remove(SOCK_PATH);
}

void sigint_handle() {
    close_srv();
    exit(0);
}

int main() {
    signal(SIGINT, sigint_handle);

    fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_PATH);

    socklen_t addrsz = sizeof(addr);
    int result = bind(fd, (struct sockaddr*)&addr, addrsz);

    char buf[MAXMSG+1];
    int running = 1;
    while (running && recvfrom(fd, buf, MAXMSG, 0, (struct sockaddr*)&addr, &addrsz)) {
        if (!strcmp(buf, "CHAU")) {
            running = 0;
        }
        printf("%s\n",buf);
        addrsz = sizeof(addr);
    }

    close_srv();

    return 0;
}