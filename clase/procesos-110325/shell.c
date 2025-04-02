#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(){
  
  int status;
  int running = 1;
  char buff[200];
  while(running){
    fgets(buff, 200, stdin);
    buff[strlen(buff)-1] = '\0';
    if (!strcmp("salir", buff)){
      running = 0;
    } else {
      pid_t pid = fork();
      if(!pid) {
        char *args[255];
        char *tok = strtok(buff, " ");
        int cnt_args = 0;
        while (tok) {
          int cnt = strlen(tok);
          char *cur_tok = malloc(sizeof(char)*(cnt+1)); // free?
          strcpy(cur_tok, tok);
          args[cnt_args++] = cur_tok;
          tok = strtok(NULL, " ");
        }
        args[cnt_args] = NULL;
        int stat = execv(args[0],args);
        printf("EXEC FAILED\n");
      }
      else{
        wait(&status);
      }
    }
    
  }
  return 0;
}