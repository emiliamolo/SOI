#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handle_zero_div(int signum) {
  puts("HANDLING!");
  exit(1);
}

int main(){
  void (*signalReturn)(int);
  signalReturn = signal(SIGFPE,handle_zero_div);

  int a = 10;
  int b = a/0;

  return 0;
}