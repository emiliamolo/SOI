#include <stdio.h> 
#include <signal.h> 
#include <stdlib.h> 
#include <unistd.h> 

void INThandler(int); 

int main(void) { 
    struct sigaction act;
    act.sa_handler = INThandler;
    act.sa_flags = SA_NODEFER;
    sigaction(SIGINT, &act, NULL);
    
    while (1) 
        sleep(10);
} 
void INThandler(int sig) { 
    char c; 
    signal(SIGINT, SIG_DFL);
    printf("OUCH, did you hit Ctrl-C?\n" 
            "Do you really want to quit? [y/n] "); 
    c = getchar(); 
    if (c == 'y' || c == 'Y') 
        exit(0); 
    else {
        struct sigaction act;
        act.sa_handler = INThandler;
        act.sa_flags = SA_NODEFER;
        sigaction(SIGINT, &act, NULL);
    }
    getchar();
}