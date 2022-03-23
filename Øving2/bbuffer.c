#include "bbuffer.h"
#include "sem.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct BNDBUF
{
    unsigned int size;
    int *newest;
    int *oldest;

    SEM *semBufferFull;
    SEM *semBufferEmpty;

    int *buffer;

} BNDBUF;

BNDBUF *bb_init(unsigned int size)
{
    /* Initialize bbuffer structure
    how to use: struct BNDBUF *bbuffer = bb_init(BNDBUFSIZE) */

    BNDBUF *bndbuf = malloc(sizeof(struct BNDBUF));
    if (bndbuf == NULL)
    {
        printf("Failed to allocate bndbuf\n");
        free(bndbuf);
    }

    bndbuf->buffer = (int *)malloc(sizeof(int) * size);

    bndbuf->size = size;
    bndbuf->newest = bndbuf->buffer;
    bndbuf->oldest = bndbuf->buffer;
    bndbuf->semBufferFull = sem_init(0);
    bndbuf->semBufferEmpty = sem_init(size);

    return bndbuf;
}
void bb_del(BNDBUF *bb)
{
    sem_del(bb->semBufferEmpty);
    sem_del(bb->semBufferFull);
    free(bb->buffer);

    free(bb);
}

int bb_get(BNDBUF *bb)
{
    P(bb->semBufferFull);
    V(bb->semBufferEmpty);

    int returnInt = *bb->oldest;
    bb->oldest++;
    if (bb->oldest == bb->buffer + bb->size)
        bb->oldest = bb->buffer;
    return returnInt;
}

void bb_add(BNDBUF *bb, int fd)
{
    P(bb->semBufferEmpty);
    V(bb->semBufferFull);

    *bb->newest = fd;
    bb->newest++;
    if (bb->newest == bb->buffer + bb->size)
        bb->newest = bb->buffer;
}