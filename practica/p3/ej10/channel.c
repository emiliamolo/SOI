#include <semaphore.h>
#include "channel.h"

void channel_init(struct channel *c){
    sem_init(&c->hay_lector, 0, 0);
    sem_init(&c->hay_lugar, 0, 1);
    sem_init(&c->hay_valor, 0, 0);
}

void chan_write(struct channel *c, int v){
    sem_wait(&c->hay_lugar);
    sem_wait(&c->hay_lector);
    c->val = v;
    sem_post(&c->hay_valor);
}

int chan_read(struct channel *c){
    sem_post(&c->hay_lector);
    sem_wait(&c->hay_valor);
    int v = c->val;
    sem_post(&c->hay_lugar);
    return v;
}