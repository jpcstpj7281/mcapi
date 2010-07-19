/*
 * Copyright (c) 2006-2008
 * Author: Weiming Zhou
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  
 */
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
#include "mcapi_threads.h"

typedef struct THREAD_PARA_st {
    int    nFlag;
    pthread_mutex_t *pMutex;
    sem_t            sem;
    THREADFUNC       func;
    void             *pArg;
} THREAD_PARA;

typedef struct MCAPI_THREAD_st {
    pthread_t        tid;      /* 线程ID */
    INT              nState;   /* 线程状态 */
    pthread_mutex_t  mutex;    /* 用于创建时处于挂起态的互斥 */
    sem_t            sem;     /* 用于恢复创建时就挂起的线程 */
    THREADFUNC       func;     /* 线程入口函数 */
    void            *pArg;     /* 线程入口函数的参数 */
} MCAPI_THREAD;

void *LinuxThreadFunc(void *pArg)
{
    THREADFUNC  func;
    THREAD_PARA *pPara;


    pPara = (THREAD_PARA *)pArg;

    func = pPara->func;

    if ( pPara->nFlag == MCAPI_THREAD_SUSPEND )
    {
        sem_wait(&(pPara->sem));
    }

    (*func)(pPara->pArg);

    return NULL;
}



/** 创建线程的函数

    @param  ThreadFunc func - 线程入口函数
    @param  void * args - 用作线程入口函数的参数
    @param  INT nFlag - 创建时的状态，各种状态定义如下：
                            MCAPI_THREAD_RUNNING      1
                            MCAPI_THREAD_EXIT         2
                            MCAPI_THREAD_SUSPEND      4
    @return NGTHREAD * - 成功返回NGTHREAD指针，失败返回NULL。
*/
HANDLE MCapi_CreateThread(THREADFUNC func, void *args, INT nFlag)
{
    THREAD_PARA   *pPara;
    MCAPI_THREAD  *pThread;
    pthread_t      tid;
    pthread_attr_t attr;

    pThread = (MCAPI_THREAD *)malloc(sizeof(MCAPI_THREAD));
    if ( pThread == NULL )
    {
        return NULL;
    }

    pPara = (THREAD_PARA *)malloc(sizeof(THREAD_PARA));
    if ( pPara == NULL )
    {
        free(pThread);
        return NULL;
    }

    /* Initialize and set thread detached attribute */
    (void)pthread_attr_init(&attr);
    (void)pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


    pThread->nState = nFlag;
    pThread->pArg = args;
    pThread->func = func;

    pthread_mutex_init(&(pThread->mutex), NULL);
    sem_init(&(pThread->sem), 0, 0);

    pPara->func = func;
    pPara->nFlag = nFlag;
    pPara->pArg = args;
    pPara->sem = pThread->sem;
    pPara->pMutex = &(pThread->mutex);

    if ( pthread_create( &tid, NULL, LinuxThreadFunc, (void *)pPara) != 0 )
    {
        (void)pthread_attr_destroy(&attr);
        free(pThread);
        return NULL;
    }

    pThread->tid = tid;

    /* Free attribute */
    (void)pthread_attr_destroy(&attr);
    
    return pThread;
}

/** 等待一个线程结束

    @param  NGTHREAD * pThread - 线程指针
    @param  INT nMilliSeconds - 等待时间
    @return void - 无
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


/** 挂起一个线程（暂未实现）

    @param  NGTHREAD * pThread - 线程指针
    @return void - 无
*/
void MCapi_SuspendThread(HANDLE hThread)
{
    MCAPI_THREAD *pThread;

    pThread = (MCAPI_THREAD *)hThread;
    (void)pThread;
    return;
}


/** 恢复线程执行
    暂时只能回复创建时就挂起的线程

    @param  NGTHREAD * pThread - 线程
    @return void - 无
*/
void MCapi_ResumeThread(HANDLE hThread)
{
    MCAPI_THREAD  *pThread;
    pThread = (MCAPI_THREAD *)hThread;

    if ( pThread->nState == MCAPI_THREAD_SUSPEND )
    {
        (void)sem_post(&(pThread->sem));

        pThread->nState = MCAPI_THREAD_RUNNING;
    }
    return;
}


/** 关闭线程

    @param  NGTHREAD * pThread - 线程指针
    @return void - 无
*/
void MCapi_CloseThread(HANDLE hThread)
{
    MCAPI_THREAD *pThread;

    pThread = (MCAPI_THREAD *)hThread;
    if ( pThread != NULL )
    {
        sem_destroy(&(pThread->sem));
        pthread_mutex_destroy(&(pThread->mutex));
        free(pThread);
    }
}


typedef struct MCAPI_EVENT_st {
    pthread_mutex_t   mutex;
    sem_t             sem;
} MCAPI_EVENT;

HANDLE EventCreate()
{
    MCAPI_EVENT  *pEvent = (MCAPI_EVENT *)malloc(sizeof(MCAPI_EVENT));
    if ( pEvent == NULL )
    {
        return NULL;
    }
    pthread_mutex_init(&(pEvent->mutex), NULL);
    sem_init(&(pEvent->sem), NULL);
    
    return (HANDLE)pEvent;
}

void WaitEvent(HANDLE hEvent) 
{
    MCAPI_EVENT *pEvent;

    pEvent = (MCAPI_EVENT *)hEvent;

    sem_wait(&(pEvent->sem));

    return;
}

void SendEvent(HANDLE hEvent)  
{
    MCAPI_EVENT *pEvent;

    pEvent = (MCAPI_EVENT *)hEvent;

    sem_post(&(pEvent->sem));
    return;
}

void EventClose(HANDLE hEvent)
{
    MCAPI_EVENT *pEvent;

    pEvent = (MCAPI_EVENT *)hEvent;

    sem_destroy(&(pEvent->sem));
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
	int    nCreateFlag;

	nCreateFlag = 0;
	switch (nFlag)
	{
	case MCAPI_THREAD_SUSPEND:
		nCreateFlag = CREATE_SUSPENDED;
		break;
	case MCAPI_THREAD_RUNNING:
		nCreateFlag = 0;
		break;
	default:
		break;
	}


    hThread = (HANDLE)_beginthreadex( NULL, 0, 
        func, args, nCreateFlag , NULL );

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
