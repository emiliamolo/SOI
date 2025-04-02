#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
int main(){
    int continuar = 1;
    char buff[300];
    int status;
    while(continuar){
        fgets(buff, 300, stdin);
        buff[strlen(buff)-1] = '\0';
        if (strcmp(buff, "salir") == 0) continuar = 0;
        else{
            pid_t pid = fork();
            if (pid == 0){
                // dividir argumentos
                char* argumentos[100];
                argumentos[0] = strtok(buff," ");
                int i=0;
                while(argumentos[i] != NULL){;
                    argumentos[++i] = strtok(NULL, " ");
                }
                for (int i = 0; argumentos[i]; i++) {
                    if (i && !strcmp(argumentos[i-1], ">")){
                        int fd_ind = open(argumentos[i], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                        dup2(fd_ind, 1);
                        argumentos[i-1] = NULL;
                    }
                }
                execvp(argumentos[0], argumentos);
                //close(fd_ind);
                printf("ERROR 🚀");
                return 1;
            }else{
                wait(&status);
            }
        }
    }
    return 0;
}