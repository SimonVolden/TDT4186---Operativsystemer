#include "sem.h"
#include <pthread.h>
#include <errno.h>

typedef struct SEM
{
    volatile int counter;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

} SEM;

SEM *sem_init(int initVal)
{

    SEM *sem = malloc(sizeof(struct SEM));
    if (!sem)
        goto Error1;
    sem->counter = initVal;

    errno = pthread_mutex_init(&sem->mutex, NULL);
    if (!errno)
        goto Error2;

    errno = pthread_cond_init(&sem->cond, NULL);
    if (!errno)
        goto Error3;

    return sem;
    // TODO: LEGG TIL ERROR

Error3:
    pthread_mutex_destroy(&sem->mutex);
Error2:
    // free(buf); //TODO: Hva skal dette være?
Error1:
    return NULL;
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

void P(SEM *sem)
{
    pthread_mutex_lock(&sem->mutex);

    // Wait for the semaphore to have a positive value.
    while (sem->counter < 1)
        pthread_cond_wait(&sem->cond, &sem->mutex);

    --sem->counter;

    // Wake up a thread that's waiting, if any.
    if (sem->counter > 0)
        pthread_cond_signal(&sem->cond);

    pthread_mutex_unlock(&sem->mutex);
}

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
