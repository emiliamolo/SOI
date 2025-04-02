#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sigchld_handler() {
    puts("SE RECIBIÓ SIGCHLD");
    exit(0);
}

int main(){
    pid_t pid = fork();
    if(!pid){
        struct sigaction act;
        act.sa_handler = sigchld_handler;
        act.sa_flags = SA_RESTART;

        sigaction(SIGCHLD, &act, NULL);

        while(1) {
            sleep(1);
        }
    }else{
        sleep(1);
        int status;
        kill(pid, SIGCHLD);
        wait(&status);
    }
    
    return 0;
}