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
        perror("Failed to allocate sem\n");
        return NULL;
    }

    sem->counter = initVal;
    int mutexStatus = pthread_mutex_init(&sem->mutex, NULL);
    int condStatus = pthread_cond_init(&sem->cond, NULL);

    if (mutexStatus || condStatus)
    {
        perror("Failed to initialize mutex\n");
        sem_del(sem);
        return NULL;
    };

    return sem;
}

int sem_del(SEM *sem)
{
    int returnValue = 0;
    int mutexStatus = pthread_mutex_destroy(&sem->mutex);
    int condStatus = pthread_cond_destroy(&sem->cond);
    if (mutexStatus || condStatus)
    {
        perror("Failed to destroy mutex\n");
        returnValue = -1;
    };
    free(sem);
    return returnValue;
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
