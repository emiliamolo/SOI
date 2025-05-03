#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define N_FILOSOFOS 5
#define ESPERA 5000000
#define BACKOFF 2
pthread_mutex_t tenedor[N_FILOSOFOS];
pthread_cond_t tenedor_ocupado[N_FILOSOFOS];
pthread_mutex_t * izq(int i) { return &tenedor[i]; }
pthread_mutex_t * der(int i) { return &tenedor[(i+1) % N_FILOSOFOS]; }
void pensar(int i)
{
  printf("Filosofo %d pensando...\n", i);
  usleep(random() % ESPERA);
}
void comer(int i)
{
  printf("Filosofo %d comiendo...\n", i);
  usleep(random() % ESPERA);
}
void tomar_tenedores(int i)
{
  pthread_mutex_lock(der(i));
  while(pthread_mutex_trylock(izq(i))){
    printf("Filosofo %d suelta tenedor derecho\n", i);
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += BACKOFF;
    pthread_cond_timedwait(&tenedor_ocupado[i], der(i), &ts);
  }
  
}
void dejar_tenedores(int i)
{
  pthread_mutex_unlock(der(i));
  pthread_cond_signal(&tenedor_ocupado[(i+1)%N_FILOSOFOS]);
  pthread_mutex_unlock(izq(i));

}
void * filosofo(void *arg)
{
  int i = arg - (void*)0;
  while (1) {
    tomar_tenedores(i);
    comer(i);
    dejar_tenedores(i);
    pensar(i);
  }
}
int main() // TODO: Ver por qué es livelock, ver si falla, etc
{
  pthread_t filo[N_FILOSOFOS];
  int i;
  for(i= 0; i < N_FILOSOFOS; i++)
    pthread_cond_init(&tenedor_ocupado[i], NULL);
  for (i = 0; i < N_FILOSOFOS; i++)
    pthread_mutex_init(&tenedor[i], NULL);
  for (i = 0; i < N_FILOSOFOS; i++)
    pthread_create(&filo[i], NULL, filosofo, i + (void*)0);
  pthread_join(filo[0], NULL);
  return 0;
}