#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "stack_unbound.h"

// gcc stack_unbound_main.c stack_unbound.c -o stack_unbound

// Ejecutar (para mostrar el race condition):
// while true; do ./stack_unbound; done

struct StackNode* root = NULL;

pthread_mutex_t mutex_stack = PTHREAD_MUTEX_INITIALIZER;

void *function1( ) {
  
  for(int i = 0; i < 1000; ++i)
  {
    //pthread_mutex_lock(&mutex_stack);
    push(&root, i);
    //pthread_mutex_unlock(&mutex_stack);
  }

}

void *function2( ) {
  
  for(int i = 0; i < 1000; ++i)
  {
    //pthread_mutex_lock(&mutex_stack);
    push(&root, i);
    //pthread_mutex_unlock(&mutex_stack);
  }

}


int main()
{
  pthread_t one, two; 
  pthread_create(&one, NULL, function1, NULL);  // create thread
  pthread_create(&two, NULL, function2, NULL);

  pthread_join(one, NULL);
  pthread_join(two, NULL);

  int  counter = 0;

  while(!isEmpty(root))
  {
    counter++;
    pop(&root);
  }

  printf("counter final: %d\n", counter);

  stackFree(root);
 
  return 0;
}