#ifndef CONCURRENT_STACK_UNBOUND_H
#define CONCURRENT_STACK_UNBOUND_H

#include <stdlib.h>
#include <pthread.h>

struct ConcurrentStack{
    struct StackNode* stack;
    pthread_mutex_t mutex;
};

void concurrent_stack_init(struct ConcurrentStack* concurrentStack);
struct StackNode* newNode(int data);
 
int concurrent_isEmpty(struct ConcurrentStack* concurrentStack);
 
void concurrent_push(struct ConcurrentStack* concurrentStack, int data);
 
int concurrent_pop(struct ConcurrentStack* concurrentStack);
 
int concurrent_top(struct ConcurrentStack* concurrentStack);

void concurrent_stackFree(struct ConcurrentStack* concurrentStack);

#endif /* CONCURRENT_STACK_H */