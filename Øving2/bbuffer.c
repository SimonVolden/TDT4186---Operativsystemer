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
    // Initialize bbuffer structure
    // how to use: struct BNDBUF *bbuffer = bb_init(BNDBUFSIZE)

    BNDBUF *bndbuf = malloc(sizeof(struct BNDBUF));
    if (bndbuf == NULL)
        printf("failed to allocate\n");

    bndbuf->buffer = (int *)malloc(sizeof(int) * size);

    bndbuf->size = size;
    bndbuf->newest = bndbuf->buffer;
    bndbuf->oldest = bndbuf->buffer;
    bndbuf->semBufferFull = sem_init(0);
    bndbuf->semBufferEmpty = sem_init(size);
    printf("SIZE: %d \n", size);
    return bndbuf;
}
void bb_del(BNDBUF *bb)
{

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
    V(bb->semBufferEmpty);

    int returnInt = *bb->oldest;
    printf("got value %d from buffer\n", returnInt);
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
    printf("added %d to buffer", fd);
    bb->newest++;
    if (bb->newest == bb->buffer + bb->size)
        bb->newest = bb->buffer;
}