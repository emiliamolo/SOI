#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
/* Producer/consumer program illustrating conditional variables */

/* Size of shared buffer */
#define BUF_SIZE 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_hay_lugar = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_hay_elemento = PTHREAD_COND_INITIALIZER;
int buffer[BUF_SIZE];					/* shared buffer */
int add=0;										/* place to add next element */
int rem=0;										/* place to remove next element */
int num=0;										/* number elements in buffer */

/* Produce value(s) */
void *producer(void *param)
{
	int i;
	for (i=1; i<=20; i++) {
		/* Insert into buffer */
		pthread_mutex_lock(&mutex);
		while(num == BUF_SIZE){
            printf("productor en el wait\n");
            pthread_cond_wait(&cond_hay_lugar, &mutex);
        }

		if (num > BUF_SIZE){
			printf("overflow\n");
			exit(1);	/* overflow */
		} 

		/* if executing here, buffer not full so add element */
		buffer[add] = i;
		add = (add+1) % BUF_SIZE;
		num++;
 		printf ("producer: inserted %d buffer size: %d\n", i, num);  fflush (stdout);
        pthread_cond_signal(&cond_hay_elemento);
		pthread_mutex_unlock(&mutex);
	}
	printf ("producer quiting\n");  fflush (stdout);
}

/* Consume value(s); Note the consumer never terminates */
void *consumer(void *param)
{
	int i;
	while (1) {
		pthread_mutex_lock(&mutex);
        while(num == 0){
            printf("Consumidor en el wait\n");
            pthread_cond_wait(&cond_hay_elemento, &mutex);
        }
		if (num < 0){
			printf("underflow\n");
			exit(1);	/* underflow */
		} 

		/* if executing here, buffer not empty so remove element */
		i = buffer[rem];
		rem = (rem+1) % BUF_SIZE;
		num--;
    	printf ("Consume value %d bufer size: %d\n", i, num);  fflush(stdout);
        pthread_cond_signal(&cond_hay_lugar);
		pthread_mutex_unlock(&mutex);
	}
}

int main (int argc, char *argv[])
{
	pthread_t tid1, tid2;		/* thread identifiers */
	int i;

	/* create the threads; may be any number, in general */
	if (pthread_create(&tid1,NULL,producer,NULL) != 0) {
		fprintf (stderr, "Unable to create producer thread\n");
		exit (1);
	}
	if (pthread_create(&tid2,NULL,consumer,NULL) != 0) {
		fprintf (stderr, "Unable to create consumer thread\n");
		exit (1);
	}
	/* wait for created thread to exit */
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	printf ("Parent quiting\n");
	return 0;

}