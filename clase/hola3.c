#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
  pid_t pid = fork();
  if (!pid) {
    printf("Soy hijo! %d\n", getpid());
    int cnt = 100000;
    while(--cnt);
  }
  else {
    printf("Soy padre %d, y su hijo es %d\n", getpid(), pid);
    int cnt = 100000;
    while(--cnt);
  }
  printf("doble %d\n",getpid());
  return 0;
}