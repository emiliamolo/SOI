#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
  int fd = open("./archivo", O_RDONLY);
  char buf[1025];
  int read_bytes = read(fd, buf, 1024);
  buf[read_bytes] = '\0';
  printf("%s",buf);
  printf("\nLEIDOS %d\n",read_bytes);
  close(fd);

  return 0;
}