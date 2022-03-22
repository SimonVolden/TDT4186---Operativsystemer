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

    printf("size of sem %d\n", sizeof(struct SEM));
    SEM *sem = malloc(sizeof(struct SEM));
    /*
    if (sem == NULL)
        goto Error1;
        */
    sem->counter = initVal;
    printf("counter set, %d \n", sem->counter);
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
    printf("sem counter: %d \n", sem->counter);
    return sem;
}
/*
void P(SEM *sem)
{
    if ((errno = pthread_cond_wait(&sem->cond, &sem->mutex)) != 0)
    {
        return;
    }
    while (sem->counter <= 0)
    {
        if ((errno = pthread_cond_wait(&sem->cond, &sem->mutex)) != 0)
        {
            pthread_mutex_unlock(&sem->mutex);
            return;
        }
        sem->counter--;
        if (errno = pthread_mutex_ulock(&sem->mutex) != 0)
            return;
    }
}
*/

int sem_del(SEM *sem)
{
    free(sem);
}

void P(SEM *sem)
{
    printf("locking value\n");
    pthread_mutex_lock(&sem->mutex);
    printf("locked value %d\n", sem->counter);
    // Wait for the semaphore to have a positive value.
    while (sem->counter < 1)
    {
        pthread_cond_wait(&sem->cond, &sem->mutex);
        printf("got signaled\n");
    }
    // Wake up a thread that's waiting, if any.
    // if (sem->counter > 0)
    sem->counter--;
    pthread_mutex_unlock(&sem->mutex);
    printf("unlocked value\n");
}

void V(SEM *sem)
{
    pthread_mutex_lock(&sem->mutex);
    sem->counter++;
    pthread_mutex_unlock(&sem->mutex);
    pthread_cond_signal(&sem->cond);
}
/*
void V(SEM *sem)
{
    if ((errno = pthread_mutex_lock(&sem->mutex)) != 0)
    {
        return;
    }
    sem->counter++;

    if ((errno = pthread_cond_broadcast(&sem->cond)) != 0)
        return;
    if ((errno = pthread_mutex_unlock(&sem->mutex) != 0))
        return;
}

*/
