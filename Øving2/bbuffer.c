#include "bbuffer.h"
#include "sem.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>

/*
typedef struct Node
{
    int fd;
    struct Node *next;
}
*/

typedef struct BNDBUF
{
    unsigned int size;

    SEM semLowerLimit;
    SEM semUpperLimit;

    void *buffer_end;
    size_t capacity;

    size_t sz;
    void *head;
    void *tail;
    //
    int newest; // %10
    int oldest;

    int fds[];

} BNDBUF;

BNDBUF *bb_init(unsigned int size)
{
    SEM sem;

    BNDBUF *bndbuf = malloc(sizeof(BNDBUF));
    bndbuf->semLowerLimit = *sem_init(0);
    bndbuf->semUpperLimit = *sem_init(size);
    bndbuf->capacity = size;
    int fds_temp[size]; // Kanskje unødvendig
    bndbuf->fds[0] = fds_temp;

    if (!bndbuf)
    {
        goto Error1;
    }

    bndbuf->size = size;

    return bndbuf;

Error1:
    return ((void *)0);
}

void bb_del(BNDBUF *bb)
{

    // free(bb->buffer);
    free(bb->semLowerLimit);
    free(bb->semUpperLimit);
    free(bb->buffer_end);
    free(bb->capacity);
    free(bb->sz);
    free(bb->head);
    free(bb->tail);
    free(bb->newest);
    free(bb->oldest);
    free(bb->fds);
}

int bb_get(BNDBUF *bb)
{

    bb->fds[bb->semLowerLimit->counter]

    /*P
    V
    return fds[x] */
}

void bb_add(BNDBUF *bb, int fd)
{

    /*V
    P
    fds[x] = fd */
}