#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_sigint() {
    puts("SE RECIBIÓ SIGINT, SE REESTABLECE EL COMPORTAMIENTO DEFAULT");
    signal(SIGINT, SIG_DFL);
}

void handle_sigquit() {
    puts("SE RECIBIÓ SIGQUIT");
}

int main(){
    signal(SIGINT,handle_sigint);
    signal(SIGQUIT,handle_sigquit);

    while(1) {
        sleep(1000);
    }

    return 0;
}