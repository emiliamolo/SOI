#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 3
#define NCLIENTES 100
sem_t sillas;
sem_t siguiente;
pthread_barrier_t barrier;
pthread_barrier_t barrier2;
void me_cortan(int i){
  printf("Me cortan (%d)\n",i);
  sleep(1);
}
void cortando(){
  printf("Cortando\n");

}
void me_pagan(){
  printf("Me pagan\n");
}
void pagando(int i){
  printf("pagando (%d) ....  \n",i);
  sleep(1);
}

void* cliente(void* param){
  int i = param - (void*)0;

  usleep(random() % 50000000);

  if (sem_trywait(&sillas)) {
    printf("Todo ocupado, me voy!\n");
    return NULL;
  }
  printf("Me senté! (%d)\n",i);

  sem_wait(&siguiente);
  
  pthread_barrier_wait(&barrier);
  me_cortan(i);
  pthread_barrier_wait(&barrier2);

  sem_post(&sillas);
  pagando(i); // paga parado...

  return NULL;
}


void* barbero(void* _){
  while(1){;
    sem_post(&siguiente);
    pthread_barrier_wait(&barrier);
    cortando();
    pthread_barrier_wait(&barrier2);
    me_pagan();
  }
  return NULL;
}

int main(){
  sem_init(&sillas, 0, N);
  sem_init(&siguiente, 0, 0);
  pthread_barrier_init(&barrier, NULL, 2);
  pthread_barrier_init(&barrier2, NULL, 2);

  pthread_t barbero_thread, clientes[NCLIENTES];
  pthread_create(&barbero_thread, NULL, barbero, NULL);

  for(int i=0; i<NCLIENTES; i++){
    pthread_create(&clientes[i], NULL, cliente, i + (void*)0);
  }

  pthread_join(barbero_thread, NULL);
  for(int i=0; i<NCLIENTES; i++){
    pthread_join(clientes[i], NULL);
  }
  pthread_barrier_destroy(&barrier);
  pthread_barrier_destroy(&barrier2);
  return 0;
}