#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  int sec = atoi(argv[2]);
  while (1) {
    pid_t pid = fork();
    if (!pid) {
      execl(argv[1],"",NULL);
    }
    sleep(sec);
  }
  
  return 0;
}