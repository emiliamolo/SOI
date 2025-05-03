#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define N_FILOSOFOS 5
#define ESPERA 5000000
pthread_mutex_t tenedor[N_FILOSOFOS];
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
  if (i == 0){
    pthread_mutex_lock(izq(i));
    pthread_mutex_lock(der(i));
  }else{
    pthread_mutex_lock(der(i));
    pthread_mutex_lock(izq(i));
  }
  
}
void dejar_tenedores(int i)
{
  if (i == 0){
    pthread_mutex_unlock(izq(i));
    pthread_mutex_unlock(der(i));
  }else{
    pthread_mutex_unlock(der(i));
    pthread_mutex_unlock(izq(i));
  }
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
  for (i = 0; i < N_FILOSOFOS; i++)
    pthread_mutex_init(&tenedor[i], NULL);
  for (i = 0; i < N_FILOSOFOS; i++)
    pthread_create(&filo[i], NULL, filosofo, i + (void*)0);
  pthread_join(filo[0], NULL);
  return 0;
}

/*
Supongamos que hay un deadlock. En ese caso, todos los locks deben estar tomados y todos los filosofos tienen
que tener exactamente un tenedor. Si alguno tiene 2 tenedores, eventualmente termina y no hay deadlock. Y si tiene
0 tenedores, hay dos casos:
- Si es el que está a la izquierda del zurdo, entonces el zurdo tiene un tenedor. Luego, el que está
  a su derecha del zurdo debe tener dos, porque sino no hubiera agarrado el de la derecha del zurdo, y dijimos
  que esto no produce deadlock ya que el de dos tenedores eventualmente termina.
- Si no, a su derecha tiene a alguien diestro. Al tener 0 tenedores, el de su derecha tiene su tenedor izquierdo,
  y por ende también su tenedor derecho por ser diestro. Luego tiene 2 tenedores y eventualmente termina.
Asumieremos entonces c/u tiene un sólo tenedor.
Al tener un filosofo zurdo, esto no puede suceder, ya que si el zurdo tomo el tenedor a su izquierda,
el filosofo a su izquierda, que es diestro, no puede tomar el tenedor, porque lo 
tomo el zurdo, entonces tiene 0 tenedores que dijimos no produce deadlock.
Por lo tanto, no puede haber deadlock.
*/