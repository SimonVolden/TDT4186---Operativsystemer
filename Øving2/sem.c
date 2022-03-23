#include "sem.h"
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct SEM
{
    volatile int counter;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

} SEM;

SEM *sem_init(int initVal)
{
    SEM *sem = (SEM *)malloc(sizeof(struct SEM));
    if (sem == NULL)
    {
        printf("Failed to allocate sem\n");
        free(sem);
    }

    sem->counter = initVal;
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
    // TODO: error handling

    return sem;
}

int sem_del(SEM *sem)
{
    free(sem);
}

void P(SEM *sem)
{
    pthread_mutex_lock(&sem->mutex);
    if (sem->counter < 1)
    {
        pthread_cond_wait(&sem->cond, &sem->mutex);
    }
    sem->counter--;
    pthread_mutex_unlock(&sem->mutex);
}

void V(SEM *sem)
{
    pthread_mutex_lock(&sem->mutex);
    sem->counter++;
    pthread_mutex_unlock(&sem->mutex);

    pthread_cond_signal(&sem->cond);
}
