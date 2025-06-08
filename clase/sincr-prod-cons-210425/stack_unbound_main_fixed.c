#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "concurrent_stack_unbound.h"
#define ELEMENT_COUNT 1000

struct thread_config{
  size_t element_count;
  struct ConcurrentStack* stack;
};
// gcc stack_unbound_main.c stack_unbound.c concurrent_stack_unbound.c


void * function1(void* tc) {
  
  for(int i = 0; i < ELEMENT_COUNT; ++i)
  {
    concurrent_push(((struct thread_config*)tc)->stack, i);
  }

}

void *function2(void* tc) {
  
  for(int i = 0; i < ELEMENT_COUNT; ++i)
  {
    concurrent_push(((struct thread_config*)tc)->stack, i);
  }

}


int main()
{
  struct ConcurrentStack concurrentStack;
  concurrent_stack_init(&concurrentStack);

  struct thread_config tc;
  tc.element_count = 0;
  tc.stack = &concurrentStack;
  
  pthread_t one, two; 
  pthread_create(&one, NULL, function1, (void*) &tc);  // create thread
  pthread_create(&two, NULL, function2, (void*) &tc);

  pthread_join(one, NULL);
  pthread_join(two, NULL);

  int counter = 0;

  while(!concurrent_isEmpty(tc.stack))
  {
    counter++;
    concurrent_pop(tc.stack);
  }

  printf("counter final: %d\n", counter);

  concurrent_stackFree(&concurrentStack);
 
  return 0;
}