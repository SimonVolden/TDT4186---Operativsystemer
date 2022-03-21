#include "bbuffer.h"
#include "sem.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>

typedef struct BNDBUF
{
    unsigned int size;
    int newest;
    int oldest;

    SEM *semBufferFull;
    SEM *semBufferEmpty;
    pthread_mutex_t mutex;

    int *buffer[];

} BNDBUF;

BNDBUF *bb_init(unsigned int size)
{

    BNDBUF *bndbuf = malloc(sizeof(struct BNDBUF));

    bndbuf->size = size;
    bndbuf->newest = 0;
    bndbuf->oldest = 0;
    bndbuf->semBufferFull = sem_init(0);
    bndbuf->semBufferEmpty = sem_init(size);
    if (pthread_mutex_init(&bndbuf->mutex, NULL) != 0)
    {
        free(bndbuf);
    }
}
void bb_del(BNDBUF *bb)
{

    if (pthread_mutex_destroy(&bb->mutex) == 0)
    {
        // tror ikke det må noe inn her egentlig, kanskje errors?
    }
    if (sem_del(bb->semBufferEmpty) == 0)
    {
        // tror ikke det må noe inn her egentlig, kanskje errors?
    }
    if (sem_del(bb->semBufferFull) == 0)
    {
        // tror ikke det må noe inn her egentlig, kanskje errors?
    }
    free(bb);
}

int bb_get(BNDBUF *bb)
{
    P(bb->semBufferFull);
pthread:
    pthread_mutex_lock(&bb->mutex);
    int returnInt = bb->buffer[bb->oldest];
    bb->oldest = bb->oldest + 1 % bb->size;
    pthread_mutex_unlock(&bb->mutex);
    V(bb->semBufferEmpty);
    return returnInt;
}

void bb_add(BNDBUF *bb, int fd)
{

    P(&bb->semBufferEmpty);
    bb->buffer[bb->newest] = fd;
    bb->newest++;
    V(bb->semBufferFull);
}