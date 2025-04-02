#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(){
  void (*signalReturn)(int);
  signalReturn = signal(SIGSTOP,SIG_IGN); // SIGSTOP no puede ser ignorada

  raise(SIGSTOP);

  return 0;
}