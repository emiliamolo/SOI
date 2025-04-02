#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
  pid_t pid = fork();
  int status;
  if(!pid){
    printf("Soy el hijo\n");
    sleep(2);
  }
  else{
    wait(&status);
    printf("Soy padre, mi hijo terminó con %d\n",status);
  }
  return 0;
}