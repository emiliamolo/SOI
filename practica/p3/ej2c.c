#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N_FILOSOFOS 5
//#define ESPERA 5000000
#define ESPERA 500000

pthread_mutex_t tenedor[N_FILOSOFOS];
sem_t dijkstra_sem;
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
  pthread_mutex_lock(izq(i));
}
void dejar_tenedores(int i)
{
  pthread_mutex_unlock(der(i));
  pthread_mutex_unlock(izq(i));
}
void * filosofo(void *arg)
{
  int i = arg - (void*)0;
  while (1) {
    sem_wait(&dijkstra_sem);
    tomar_tenedores(i);
    comer(i);
    dejar_tenedores(i);
    sem_post(&dijkstra_sem);
    pensar(i);
  }
}
int main()
{
  pthread_t filo[N_FILOSOFOS];
  int i;
  for (i = 0; i < N_FILOSOFOS; i++)
    pthread_mutex_init(&tenedor[i], NULL);

  sem_init(&dijkstra_sem, 0, N_FILOSOFOS - 1);
  
  for (i = 0; i < N_FILOSOFOS; i++)
    pthread_create(&filo[i], NULL, filosofo, i + (void*)0);

  pthread_join(filo[0], NULL);

  for (int i = 0; i < N_FILOSOFOS; i++)
    pthread_mutex_destroy(&tenedor[i]);
  sem_destroy(&dijkstra_sem);

  return 0;
}

/*
Para que suceda un deadlock necesitamos que un thread requiera un recurso tomado por otro y a su vez que 
este tomando otro recurso, requerido por el otro, aunque haya varios threads tomando i recursos en "cadena".
Por lo tanto, como vimos anteriormente, es necesario que todos los filósofos tengan ocupado sólo su tenedor
derecho. Esto es que, todos los filósofos no puedan ocupar su tenedor izquierdo ya que el filósofo a su izquierda
lo está ocupando. Eso sólo vale si para todos los filósofos, el que está a su izquierda está intentando comer.
Sin embargo, con N-1 filósofos intentando comer, de los N totales, significa que hay algún filósofo en la ronda
que NO va a tener un filósofo a su izquierda intentando comer, y por ende no ocupe el tenedor izquierdo (el
derecho del que no intenta comer). De esta forma, el filósofo podrá agarrar ambos tenedores y comerá,
eventualmente terminando, y evitando el deadlock.
*/