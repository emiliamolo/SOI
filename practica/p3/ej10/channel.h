#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <semaphore.h>

struct channel{
    sem_t hay_valor;
    sem_t hay_lector;
    sem_t hay_lugar;
    int val;
};

void channel_init(struct channel *c); 
void chan_write(struct channel *c, int v); 
int chan_read(struct channel *c);

#endif /* __CHANNEL_H__ */