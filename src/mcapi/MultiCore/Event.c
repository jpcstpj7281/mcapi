#ifndef _WIN32
#include <stdlib.h>
#include <pthread.h>
#include "CapiGlobal.h"
#include "Event.h"

typedef struct MCAPI_EVENT_st {
    pthread_mutex_t   mutex;
    pthread_cond_t    cond;
} MCAPI_EVENT;

HANDLE EventCreate()
{
    MCAPI_EVENT  *pEvent = (MCAPI_EVENT *)malloc(sizeof(MCAPI_EVENT));
    if ( pEvent == NULL )
    {
        return NULL;
    }
    pthread_mutex_init(&(pEvent->mutex), NULL);
    pthread_cond_init(&(pEvent->cond), NULL);
    
    return (HANDLE)pEvent;
}

void WaitEvent(HANDLE hEvent) 
{
    MCAPI_EVENT *pEvent;

    pEvent = (MCAPI_EVENT *)hEvent;

    pthread_mutex_lock(&(pEvent->mutex));
    
    pthread_cond_wait(&(pEvent->cond), &(pEvent->mutex));

    pthread_mutex_unlock(&(pEvent->mutex));
    return;
}

void SendEvent(HANDLE hEvent)  
{
    MCAPI_EVENT *pEvent;

    pEvent = (MCAPI_EVENT *)hEvent;

    pthread_mutex_lock(&(pEvent->mutex));

    pthread_cond_signal(&(pEvent->cond));

    pthread_mutex_unlock(&(pEvent->mutex));
    return;
}

void EventClose(HANDLE hEvent)
{
    MCAPI_EVENT *pEvent;

    pEvent = (MCAPI_EVENT *)hEvent;

    pthread_cond_destroy(&(pEvent->cond));
    pthread_mutex_destroy(&(pEvent->mutex));

    free(pEvent);
    return;
}



#endif /* _WIN32 */