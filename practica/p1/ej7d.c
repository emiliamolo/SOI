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
            // dividir argumentos
            char* argumentos[100];
            argumentos[0] = strtok(buff," ");
            int i=0;
            while(argumentos[i] != NULL){;
                argumentos[++i] = strtok(NULL, " ");
            }

            pid_t pid = fork();

            if (!pid) {
                char **args = argumentos;

                int fd[2];

                int piping = 1;
                for(int i=0; argumentos[i] && piping; i++){
                    if (!strcmp(argumentos[i], "|")){
                        pipe(fd);
                        pid_t pid_pipe = fork();
                        if (pid_pipe == 0){
                            dup2(fd[0], 0);
                            close(fd[1]);
                            args = argumentos+i+1;
                            
                        }
                        else{
                            dup2(fd[1], 1);
                            close(fd[0]);
                            piping = 0;
                            argumentos[i] = NULL;
                        }
                    }
                }

                for (int i = 0; args[i]; i++) {
                    if (i && !strcmp(args[i-1], ">")){
                        int fd_ind = open(args[i], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                        dup2(fd_ind, 1);
                        args[i-1] = NULL;
                    }
                }
                execvp(args[0], args);
                printf("Hubo un error ejecutando el comando %s\n",args[0]);
                return 1;
            }else{
                wait(&status);
            }
        }
    }
    return 0;
}