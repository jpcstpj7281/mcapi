#ifdef _WIN32
#include <windows.h>
#include "CapiGlobal.h"
#include <process.h>
#endif

#ifndef _WIN32
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "CapiGlobal.h"
#include "Event.h"


#define         MCAPI_THREAD_RUNNING      1
#define         MCAPI_THREAD_EXIT         2
#define         MCAPI_THREAD_SUSPEND      3



typedef struct MCAPI_THREAD_st {
    pthread_t        tid;      /* �߳�ID */
    INT              nState;   /* �߳�״̬ */
    pthread_mutex_t  mutex;    /* ���ڴ���ʱ���ڹ���̬�Ļ��� */
    pthread_cond_t   cond;     /* ���ڻָ�����ʱ�͹�����߳� */
} MCAPI_THREAD;


/** �����̵߳ĺ���

    @param  ThreadFunc func - �߳���ں���
    @param  void * args - �����߳���ں����Ĳ���
    @param  INT nFlag - ����ʱ��״̬������״̬�������£�
                            NGTHREAD_RUNNING      1
                            NGTHREAD_EXIT         2
                            NGTHREAD_SUSPEND      3
    @return NGTHREAD * - �ɹ�����NGTHREADָ�룬ʧ�ܷ���NULL��
*/
HANDLE MCapi_CreateThread(THREADFUNC func, void *args, INT nFlag)
{
    MCAPI_THREAD  *pThread;
    pthread_t      tid;
    pthread_attr_t attr;
    
    (void)nFlag;

    pThread = (MCAPI_THREAD *)malloc(sizeof(MCAPI_THREAD));
    if ( pThread == NULL )
    {
        return NULL;
    }
    /* Initialize and set thread detached attribute */
    (void)pthread_attr_init(&attr);
    (void)pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    if ( pthread_create( &tid, NULL, func, args) != 0 )
    {
        (void)pthread_attr_destroy(&attr);
        free(pThread);
        return NULL;
    }

    pThread->tid = tid;
    pThread->nState = nFlag;

    pthread_mutex_init(&(pThread->mutex), NULL);
    pthread_cond_init(&(pThread->cond), NULL);

    /* Free attribute */
    (void)pthread_attr_destroy(&attr);

    if ( nFlag == MCAPI_THREAD_SUSPEND )
    {

        pthread_mutex_lock(&(pThread->mutex));

        pthread_cond_wait(&(pThread->cond), &(pThread->mutex));

        pthread_mutex_unlock(&(pThread->mutex));
    }
    
    return pThread;
}

/** �ȴ�һ���߳̽���

    @param  NGTHREAD * pThread - �߳�ָ��
    @param  INT nMilliSeconds - �ȴ�ʱ��
    @return void - ��
*/
void MCapi_WaitThread(HANDLE hThread, INT nMilliSeconds)
{
    MCAPI_THREAD  *pThread;
    pthread_t tid;
    void *status;

    pThread = (MCAPI_THREAD *)hThread;
    (void)nMilliSeconds;
    
    tid = pThread->tid;

    (void)pthread_join(tid, &status);

    pThread->nState = MCAPI_THREAD_EXIT;
}


/** ����һ���̣߳���δʵ�֣�

    @param  NGTHREAD * pThread - �߳�ָ��
    @return void - ��
*/
void MCapi_SuspendThread(HANDLE hThread)
{
    MCAPI_THREAD *pThread;

    pThread = (MCAPI_THREAD *)hThread;
    (void)pThread;
    return;
}


/** �ָ��߳�ִ��
    ��ʱֻ�ܻظ�����ʱ�͹�����߳�

    @param  NGTHREAD * pThread - �߳�
    @return void - ��
*/
void MCapi_ResumeThread(HANDLE hThread)
{
    MCAPI_THREAD  *pThread;
    pThread = (MCAPI_THREAD *)hThread;

    if ( pThread->nState == MCAPI_THREAD_SUSPEND )
    {
        pthread_mutex_lock(&(pThread->mutex));

        pthread_cond_signal(&(pThread->cond));

        pthread_mutex_unlock(&(pThread->mutex));
    }
    return;
}


/** �ر��߳�

    @param  NGTHREAD * pThread - �߳�ָ��
    @return void - ��
*/
void MCapi_CloseThread(HANDLE hThread)
{
    MCAPI_THREAD *pThread;

    pThread = (MCAPI_THREAD *)hThread;
    if ( pThread != NULL )
    {
        pthread_cond_destroy(&(pThread->cond));
        pthread_mutex_destroy(&(pThread->mutex));
        free(pThread);
    }
}


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

typedef struct MCAPI_SEMA_st {
    sem_t    sem;
    int      nMaxCount;
} MCAPI_SEMA;


HANDLE SemaCreate(int nInitCount, int nMaxCount)
{
    MCAPI_SEMA  *pSem;

    pSem = (MCAPI_SEMA *)malloc(sizeof(MCAPI_SEMA));
    if ( pSem == NULL )
    {
        return NULL;
    }

    sem_init(&(pSem->sem), 0, nInitCount);
    pSem->nMaxCount = nMaxCount;

    return (HANDLE)pSem;
}

void SemaWait(HANDLE hSem)
{
    MCAPI_SEMA *pSem;

    pSem = (MCAPI_SEMA *)hSem;
    sem_wait(&(pSem->sem));

    return;
}

void SemaRelease(HANDLE hSem, int nIncCount)
{
    MCAPI_SEMA *pSem;
    int   i;

    pSem = (MCAPI_SEMA *)hSem;

    for ( i = 0; i < nIncCount; i++ )
    {
        sem_post(&(pSem->sem));
    }
    return;
}

void SemaClose(HANDLE hSem)
{
    MCAPI_SEMA *pSem;

    pSem = (MCAPI_SEMA *)hSem;

    sem_destroy(&(pSem->sem));
    
    free(pSem);
}



#else

HANDLE MCapi_CreateThread(THREADFUNC func, void *args, INT nFlag)
{
    HANDLE  hThread;

    hThread = (HANDLE)_beginthreadex( NULL, 0, 
        func, args, nFlag , NULL );

    return hThread;
}

void MCapi_WaitThread(HANDLE hThread, INT nMilliSeconds)
{
    WaitForSingleObject(hThread, (DWORD)nMilliSeconds);
    return;
}

void MCapi_SuspendThread(HANDLE hThread)
{
    SuspendThread(hThread);
    return;
}

void MCapi_ResumeThread(HANDLE hThread)
{
    ResumeThread(hThread);
    return;
}

void MCapi_CloseThread(HANDLE hThread)
{
    CloseHandle(hThread);
    return;
}


#endif /* _WIN32 */