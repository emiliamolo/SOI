#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>

int fd = 3;

void sigint_handler() {
    close(fd);
    exit(0);
}

int main() {/*
    char buff[100];
    if (read(fd, buff, 10) < 0) {
        fd = open("prueba.txt", O_RDONLY);
        printf("FILE DESCRIPTOR: %d\n",fd);
    }

    read(fd, buff, 10);
    printf("%s\n",buff);

    sleep(5);
    execl("./h2","./h2",NULL); // LECTURA
    */
    
    signal(SIGINT, sigint_handler);

    char buff[100];
    sprintf(buff, "HOLA %d!! ", fd);

    if (write(fd, buff, 10) < 0) {
        fd = open("prueba2.txt", O_WRONLY | O_CREAT | O_TRUNC);
        printf("FILE DESCRIPTOR: %d\n",fd);
        sprintf(buff, "HOLA %d!! ", fd);
        write(fd, buff, 10);
    }

    printf("%s\n",buff);

    sleep(5);
    execl("./h2","./h2",NULL);
    // ESCRITURA

    // Los file descriptor se mantienen abiertos tras ejecutar exec (no mueren).

    return 0;
}