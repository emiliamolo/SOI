#include <stdio.h>
#include <unistd.h>

int main() {
  printf("HOLA2 %d\n",getpid());
  execl("./get_pid","",NULL);
  printf("HOLA2 %d\n",getpid());

  return 0;
}