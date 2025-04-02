#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(){
    int fd = open("prueba.txt", 'r');
    char buff[100];
    read(fd, buff, 10);
    printf("antes: %s\n", buff);
    pid_t pid = fork();
    
    if(pid == 0){
        char buff[100];
        read(fd, buff, 3);
        printf("hijo: %s\n", buff);
    }
    else{
        char buff[100];
        read(fd, buff, 3);
        printf("padre: %s\n", buff);
    }
    // comparten file descriptors. De man 2 fork:
    /*
        •  The child inherits copies of the parent's set of open file
          descriptors.  Each file descriptor in the child refers to the
          same open file description (see open(2)) as the corresponding
          file descriptor in the parent.  This means that the two file
          descriptors share open file status flags, file offset, and
          signal-driven I/O attributes (see the description of F_SETOWN
          and F_SETSIG in fcntl(2)).
    */
    return 0;
}