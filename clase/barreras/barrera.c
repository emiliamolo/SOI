#include <pthread.h>
#include <stdio.h>

pthread_barrier_t barrier;

void* thread_func(void* arg){
    printf("Thread %d antes de la barrera!\n", (long)arg);
    pthread_barrier_wait(&barrier); // block
    printf("Thread %d despues de la barrera!\n", (long)arg);

}

int main(){
    pthread_barrier_init(&barrier, NULL, 3); // bloquea hasta que hay 3
    pthread_t threads[3];

    for(int i=0;i<3;i++){
        pthread_create(&threads[i], NULL, thread_func,(void*) i);
    }
    for(int i=0;i<3;i++){
        pthread_join(threads[i], NULL);
    }
    pthread_barrier_destroy(&barrier);
    return 0;
}