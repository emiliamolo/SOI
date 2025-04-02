#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];
    pipe(fd);

    char buf[255];

    pid_t pid = fork();

    if (!pid) {
        close(fd[1]); // Close write as child
        puts("PARENT SAYS:");

        while (read(fd[0],buf,1)) {
            putchar(buf[0]);
        }

        puts("!! TERMINÉ DE LEER !!");
        
        close(fd[0]);
    }
    else {
        close(fd[0]); // Close read as parent

        sprintf(buf,"TEST\nHOLA\nTEST\n");
        write(fd[1],buf,strlen(buf));

        sleep(2);

        sprintf(buf,"\nDE VUELTA\nTEST\nDE VUELTA\n");
        write(fd[1],buf,strlen(buf));

        sleep(2);

        close(fd[1]);
        
        puts("EL PADRE CERRÓ");
    }


    return 0;
}