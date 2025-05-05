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
int main()
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

/*
Puede no funcionar porque se produce un livelock. En efecto, si todos los filósofos toman su
tenedor derecho al mismo tiempo tal que observen que no pueden tomar su tenedor izquierdo,
soltarían el derecho esperando una cantidad de tiempo fija. Si el orden en el cual termina
cada uno su espera es el mismo (con mismo tiempo de espera c/u) entonces todos volverían
a tomar su tenedor derecho impidiendo que alguno tome su izquierdo, repitiéndose el problema.
En otras palabras, si todos toman su tenedor derecho al mismo tiempo, y esperan el mismo tiempo
con la intención de que otro tome el tenedor antes, puede suceder un livelock, donde los 
filósofos NO pueden comer porque todos están intentando dejar que otro coma al soltar su tenedor.

En la práctica, al menos con un programa, esto es poco probable que se prolongue de forma 
indefinida, ya que sería poco común que el orden en que los threads resumen su ejecución, incluso
con el mismo tiempo transcurrido, sea el mismo cada vez. Es por esto que vemos que el programa
eventualmente funciona y todos pueden comer. Además, usando variables de condición se le 
avisa a otro filósofo que uno de los tenedores se encuentra disponible para agarrar, lo cual
posiblemente evita aún más que se produzca un livelock.
Además, claramente no es eficiente porque los filósofos esperan un tiempo de forma no coordinada
para seguir comiendo, generando mayores demoras por la competencia sobre los tenedores.
*/