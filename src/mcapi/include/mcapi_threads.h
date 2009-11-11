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

/* 
 *	mcapi_threads.h
 *
 *	DESCRIPTION
 *		Interface for  thread, event, semaphore interface.
 *
 *	HISTORY
 *		11-11-2009	create by zhouweiming.
 *
 */
#ifndef __MCAPI_THREADS_H__
#define __MCAPI_THREADS_H__


#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
typedef unsigned int (_stdcall *THREADFUNC)(void *pArgs);
#else
typedef unsigned int (*THREADFUNC)(void *pArgs);
#endif

typedef struct  TASK_st {
    THREADFUNC      func;         //任务执行函数
    void            *pArg;        //任务函数的参数
    DWORD           time_consume; //任务执行的耗时
} TASK;

#define     EVENT           HANDLE
#define     SEMAPHORE       HANDLE

#if defined(_WIN32)

#define EventCreate()       CreateEvent(NULL, TRUE, FALSE, NULL)
#define WaitEvent(x)        (void)WaitForSingleObject((x), INFINITE)
#define SendEvent(x)        (void)SetEvent(x)
#define EventClose(x)       (void)CloseHandle(x)

#define SemaCreate(x,y)     CreateSemaphore(NULL,x,y,NULL)
#define SemaWait(x)         WaitForSingleObject(x,INFINITE)
#define SemaRelease(x,y)    ReleaseSemaphore(x,y,NULL)
#define SemaClose(x)        CloseHandle(x)

#define CAPI_Yield()        SwitchToThread()

#else /* _WIN32 */

#define         INFINITE                  -1

HANDLE EventCreate();
void WaitEvent(HANDLE hEvent);
void SendEvent(HANDLE hEvent); 
void EventClose(HANDLE hEvent);

HANDLE SemaCreate(int nInitCount, int nMaxCount);
void SemaWait(HANDLE hSem);
void SemaRelease(HANDLE hSem, int nIncCount);
void SemaClose(HANDLE hSem);

#define CAPI_Yield()        sched_yield()

#endif /* _WIN32 */


#ifdef _WIN32
#define    TlsGetValueFunc   TlsGetValue
#define    TlsSetValueFunc   TlsSetValue
#else
#define    TlsGetValueFunc   pthread_getspecific
#define    TlsSetValueFunc   pthread_setspecific
#define    TlsFree(x)        pthread_key_delete(x)
#endif


#define         MCAPI_THREAD_RUNNING      1
#define         MCAPI_THREAD_EXIT         2
#define         MCAPI_THREAD_SUSPEND      3


HANDLE MCapi_CreateThread(THREADFUNC func, void *args, INT nFlag);
void MCapi_WaitThread(HANDLE hThread, INT nMilliSeconds);
void MCapi_SuspendThread(HANDLE hThread);
void MCapi_ResumeThread(HANDLE hThread);
void MCapi_CloseThread(HANDLE hThread);



#ifdef __cplusplus
}
#endif

#endif /* __MCAPI_THREADS_H__ */