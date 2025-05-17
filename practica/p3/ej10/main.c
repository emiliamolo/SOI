#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "channel.h"

#define NUM_PRODUCER 3
#define NUM_CONSUMER 3
#define ELEMENTS_EACH 10

struct channel c;

void *producer(void *param)
{
    int id = param - (void*)0;
    int idx = id*ELEMENTS_EACH + 1;
	int i;
	for (i=idx; i<=idx+ELEMENTS_EACH; i++) {
		/* Insert into buffer */
		chan_write(&c, i);
 		printf ("producer %d: inserted %d\n", id, i);
	}
	printf ("producer quiting\n");
}

/* Consume value(s); Note the consumer never terminates */
void *consumer(void *param)
{
    int id = param - (void*)0;
	int i;
	while (1) {
		i = chan_read(&c);
    	printf ("Consumer %d consumed value %d\n", id, i);
	}
}

int main (int argc, char *argv[])
{
	pthread_t prods[NUM_PRODUCER], cons[NUM_CONSUMER];
	int i;
    
    channel_init(&c);
    for (int i=0; i < NUM_PRODUCER; i++){
        if (pthread_create(&prods[i],NULL,producer,i+(void*)0) != 0) {
            fprintf (stderr, "Unable to create producer thread\n");
            exit (1);
	    }
    }
	for (int i=0; i< NUM_CONSUMER; i++){
        if (pthread_create(&cons[i],NULL,consumer,i+(void*)0) != 0) {
            fprintf (stderr, "Unable to create consumer thread\n");
            exit (1);
        }
    }

    for (int i=0; i<NUM_PRODUCER; i++){
        pthread_join(prods[i], NULL);
    }
    for (int i=0; i<NUM_CONSUMER; i++){
        pthread_join(cons[i], NULL);
    }
	printf ("Parent quiting\n");

	return 0;

}

/*
La mayor desventaja es que a pesar de haber múltiples productores, ninguno puede 
producir mientras haya un valor producido, quitando cualquier beneficio de tener
múltiples productores para adelantar su trabajo. Lo mismo sucede con tener múltiples
consumidores, ya que no se tiene la ventaja de tener múltiples threads que consuman
varios elementos al mismo tiempo acelerando su procesamiento.

A pesar de todo esto, como ventaja tiene que ambos threads productor y consumidor
cuando hay uno de cada lado continúan si cada uno sabe que el otro recibió / le
mandó el valor. De esta forma, se crea una especie de barrera que garantiza que
se mandó un mensaje entre ambos, lo cual puede ser útil en algunas circunstancias
como confirmar la llegada de mensajes, o evitar producir valores sin que haya
consumidores (por ejemplo, que valores dependan del consumidor, capaz).
*/