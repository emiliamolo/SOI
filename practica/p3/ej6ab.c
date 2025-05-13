#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t tabaco, papel, fosforos, otra_vez;

void agente()
{
  while (1) {
    sem_wait(&otra_vez);
    int caso = random() % 3;
    if (caso != 0) sem_post(&fosforos);
    if (caso != 1) sem_post(&papel);
    if (caso != 2) sem_post(&tabaco);
  }
}

void fumar(int fumador)
{
  printf("Fumador %d: Puf! Puf! Puf!\n", fumador);
  sleep(1);
}

void * fumador1(void *arg)
{
  while (1) {
    sem_wait(&papel);
    sem_wait(&tabaco);
    fumar(1);
    sem_post(&otra_vez);
  }
}

void * fumador2(void *arg)
{
  while (1) {
    sem_wait(&tabaco);
    sem_wait(&fosforos);
    fumar(2);
    sem_post(&otra_vez);
  }
}

void * fumador3(void *arg)
{
  while (1) {
    sem_wait(&fosforos);
    sem_wait(&papel);
    fumar(3);
    sem_post(&otra_vez);
  }
}
int main()
{
  pthread_t s1, s2, s3;
  sem_init(&tabaco, 0, 0);
  sem_init(&papel, 0, 0);
  sem_init(&fosforos, 0, 0);
  sem_init(&otra_vez, 0, 1);
  pthread_create(&s1, NULL, fumador1, NULL);
  pthread_create(&s2, NULL, fumador2, NULL);
  pthread_create(&s3, NULL, fumador3, NULL);
  agente();
  return 0;
}

/*
  Se produce un deadlock muy rápidamente porque puede pasar que un fumador que no tiene los dos ingredientes
  restantes tome uno de los provistos por el agente (pero no los dos), causando que el que sí los tiene no
  pueda tomar el faltante para que pueda fumar. Así, el primero no puede fumar porque le falta un ingrediente
  ya que el agente no lo proveyó, como tampoco puede fumar el segundo porque le falta un ingrediente ya que se lo
  "sacó" el primero. El agente tampoco es avisado de que debe volver a proveer, ya que ninguno puede fumar.
*/

/*
  No se pueden ordenar los recursos para evitar el problema. En efecto, supongamos que en todos los agentes
  que no tienen tabaco, primero intentan tomar tabaco. Entonces podría pasar que el agente provea tabaco y fósforos,
  y el que tiene fósforos tome el tabaco, impidiendo que el que tiene papel de tomar los fósforos, que 
  lo tomaría después del tabaco. Y el que tomó el tabaco tampoco puede fumar porque el agente no provee papel,
  que no tiene.
  Supongamos en cambio que ordenamos ahora que en los dos que tiene tabaco, uno lo tome primero, y el otro segundo.
  Sea que el que tiene papel tome tabaco primero (luego fósforos) y el que tiene fósforos tome segundo (primero papel),
  sin pérdida de generalidad. Entonces, si el agente provee papel y tabaco, podría pasar que el que 
  tiene papel tome primero el tabaco, haya un cambio de contexto, y el que tiene fósforos tome el papel. Así, 
  el que debería fumar, el que tiene fósforos, no puede tomar luego el tabaco porque lo tomó el otro, 
  produciéndose un deadlock.
  De esta forma, en todos los casos, hay un par de fumadores con un orden que lleva a un deadlock.
*/