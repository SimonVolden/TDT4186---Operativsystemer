#include "bbuffer.h"
#include "sem.h"
#include <errno.h>
#include <stdio.h>

typedef struct BNDBUF
{

    unsigned int size;

    void *buffer;
    void *buffer_end;
    size_t capacity;
    size_t count;
    size_t sz;
    void *head;
    void *tail;

    int newest; // %10
    int oldest;

} BNDBUF;

BNDBUF *bb_init(unsigned int size)
{

    BNDBUF *bndbuf = malloc(sizeof(BNDBUF));

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
    free(bb->buffer);
    free(bb->capacity);
    free(bb->count);
    free(bb->sz);
    free(bb->head);
    free(bb->tail);
}

int bb_get(BNDBUF *bb)
{
}