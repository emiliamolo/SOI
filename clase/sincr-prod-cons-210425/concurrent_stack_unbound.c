// C program for linked list implementation of stack
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "concurrent_stack_unbound.h"
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "stack_unbound.h"
// source: https://www.geeksforgeeks.org/stack-data-structure-introduction-program/

void concurrent_stack_init(struct ConcurrentStack* concurrentStack)
{
    concurrentStack->stack = NULL;
    pthread_mutex_init(&concurrentStack->mutex, NULL);
}
 
int concurrent_isEmpty(struct ConcurrentStack* concurrentStack)
{
    pthread_mutex_lock(&(concurrentStack->mutex));
    int result = isEmpty(concurrentStack->stack);
    pthread_mutex_unlock(&concurrentStack->mutex);
    return result;
}
 
void concurrent_push(struct ConcurrentStack* concurrentStack, int data)
{
    pthread_mutex_lock(&(concurrentStack->mutex));
    push(&concurrentStack->stack, data);
    pthread_mutex_unlock(&concurrentStack->mutex);
}
 
int concurrent_pop(struct ConcurrentStack* concurrentStack)
{
    pthread_mutex_lock(&(concurrentStack->mutex));
    int resultado = pop(&concurrentStack->stack);
    pthread_mutex_unlock(&concurrentStack->mutex);
    return resultado;
}
 
int concurrent_top(struct ConcurrentStack* concurrentStack)
{
    pthread_mutex_lock(&(concurrentStack->mutex));
    int resultado = top(concurrentStack->stack);
    pthread_mutex_unlock(&concurrentStack->mutex);
    return resultado;
}

void concurrent_stackFree(struct ConcurrentStack* concurrentStack)
{
  // we free the stack just popping all the elements
  pthread_mutex_lock(&(concurrentStack->mutex));
  stackFree(concurrentStack->stack);
  pthread_mutex_unlock(&(concurrentStack->mutex));
}
