#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

void handle(int sig){
    printf("Hola\n");   
    exit(0);
}

int main(){
    signal(SIGUSR1, handle);
    pid_t pid = fork();

    if(pid == 0){
        while(1);
    }
    else{
        kill(pid, SIGUSR1);
        while(1);
    }
    return 0;
}