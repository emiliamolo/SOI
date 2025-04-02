#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handle_stop(int signum) {
  puts("HANDLING SIGSTOP!");
  exit(1);
}

int main(){
  void (*signalReturn)(int);
  signalReturn = signal(SIGSTOP,handle_stop);

  raise(SIGSTOP);

  printf("REANUDA Y TERMINA\n");

  return 0;
}