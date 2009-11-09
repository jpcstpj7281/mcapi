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
#include <process.h>
#else
#include <pthread.h>
#endif
#include <omp.h>
#include "CapiGlobal.h"
#include "CThreadPool.h"


#ifndef _WIN32
pthread_key_t TlsAlloc()
{
    pthread_key_t tid;
    pthread_key_create(&tid, NULL);
    return tid;
}
#endif

/**	线程池的线程入口函数
    用户传入的线程入口函数在这个函数中被调用
    这个函数的功能主要是给线程进行编号

	@param	void *pArgs - 一个THREADPOOL_STARTFUNC_PARMETERS类型的参数	
	@return	unsigned __stdcall - 由用户定义的返回值	
*/
#ifdef _WIN32
unsigned __stdcall ThreadPool_StartFunc(void *pArgs)
#else
void * ThreadPool_StartFunc(void *pArgs)
#endif
{
    THREADPOOL_STARTFUNC_PARMETERS *p = (THREADPOOL_STARTFUNC_PARMETERS *)pArgs;
    TlsSetValueFunc(p->dwTlsIndex, (void *)(p->nId));

    return (*(p->StartFunc))(p->pArgs);
}


/**	线程池的获取线程Id回调函数，供其他模块使用
    调用这个函数时，必须使用CThreadPool类作为参数

	@param	void *pArgs - CThreadPool类型的参数	
	@return	int - 返回从0开始的编号	
*/
int ThreadPool_GetThreadId(void *pArgs)
{
    CThreadPool *pThreadPool = (CThreadPool *)pArgs;
    return pThreadPool->GetThreadId();
}


/**	CThreadPool类的构造函数

	@return	constructor - 无	
*/
CThreadPool::CThreadPool()
{
    m_nThreadCount = omp_get_num_procs();
    m_dwTlsIndex = TlsAlloc();
    m_pThreadArray = new HANDLE[m_nThreadCount];
    m_pPara = new THREADPOOL_STARTFUNC_PARMETERS[m_nThreadCount];
    m_nSingleThreadCount = 0;
}


/**	CThreadPool类的析构函数

	@return	destructor - 无	
*/
CThreadPool::~CThreadPool()
{
    TlsFree(m_dwTlsIndex);
    delete [] m_pThreadArray;
    delete [] m_pPara;
}


/**	CThreadPool类的构造函数

	@param	THREADFUNC StartFunc - 用户的线程启动函数	
	@param	void *pArgs - 用户的线程启动函数的参数	
	@param	int nThreadCount - 需要创建的线程数量	
	@return	constructor - 无	
*/
CThreadPool::CThreadPool(THREADFUNC StartFunc, void *pArgs, int nThreadCount)
{
    m_StartFunc = StartFunc;
    m_pArgs = pArgs;
    m_dwTlsIndex = TlsAlloc();
    if ( nThreadCount == 0 )
    {
        m_nThreadCount = omp_get_num_procs();
    }
    else
    {
        m_nThreadCount = nThreadCount;
    }

    m_pThreadArray = new HANDLE[m_nThreadCount];
    m_pPara = new THREADPOOL_STARTFUNC_PARMETERS[m_nThreadCount];
    m_nSingleThreadCount = 0;

}

/**	获取线程池类的线程Id

	@return	int - 返回从0开始编号的线程Id	
*/
int CThreadPool::GetThreadId()
{
    int nId = (int)TlsGetValueFunc(m_dwTlsIndex);

    return (nId-1);
}


/**	获取线程池内的线程数量

	@return	int - 返回线程数量	
*/
int CThreadPool::GetThreadCount()
{
    return m_nThreadCount;
}


/**	创建线程池操作函数

	@param	THREADFUNC StartFunc - 用户的线程启动入口函数	
	@param	void *pArgs - 用户的线程启动入口函数的参数	
	@param	int nThreadCount - 需要创建的线程数量	
	@return	void - 无	
*/
void CThreadPool::CreateThreadPool(THREADFUNC StartFunc, void *pArgs, int nThreadCount)
{
    int i;

    m_StartFunc = StartFunc;
    m_pArgs = pArgs;
    if ( nThreadCount == 0 )
    {
        m_nThreadCount = omp_get_num_procs();
    }
    else
    {
        m_nThreadCount = nThreadCount;
    }

    for ( i = 0; i < m_nThreadCount; i++ )
    {
        m_pPara[i].dwTlsIndex = m_dwTlsIndex;
        m_pPara[i].nId = i + 1;
        m_pPara[i].pArgs = m_pArgs;
        m_pPara[i].StartFunc = m_StartFunc;

        m_pThreadArray[i] = MCapi_CreateThread(ThreadPool_StartFunc, 
            &(m_pPara[i]), MCAPI_THREAD_SUSPEND);
    }
}

/**	创建单个线程函数
    必须在调用了CreateThreadPool函数后才可调用这个函数

	@param	THREADFUNC StartFunc - 用户的线程启动入口函数	
	@param	void *pArgs - 用户的线程启动入口函数的参数	
	@return	void - 无	
*/
void CThreadPool::CreateThread(THREADFUNC StartFunc, void *pArgs)
{
    LONG nIndex = AtomicIncrement((LONG volatile *)&m_nSingleThreadCount);
    nIndex -= 1;

    m_SinglePara[nIndex].dwTlsIndex = m_dwTlsIndex;
    m_SinglePara[nIndex].nId = m_nThreadCount + nIndex + 1;
    m_SinglePara[nIndex].pArgs = m_pArgs;
    m_SinglePara[nIndex].StartFunc = m_StartFunc;

    m_pThreadArray[nIndex] = MCapi_CreateThread(ThreadPool_StartFunc, 
        &(m_SinglePara[nIndex]), MCAPI_THREAD_SUSPEND);
}



/**	执行线程池内所有线程并等待所有线程执行完

	@return	void - 无	
*/
void CThreadPool::ExecAndWait()
{
    int i;
    for ( i = 0; i < m_nThreadCount; i++ )
    {
        MCapi_ResumeThread(m_pThreadArray[i]);
    }
    for ( i = 0; i < m_nSingleThreadCount; i++ )
    {
        MCapi_ResumeThread(m_SingleThread[i]);
    }
    WaitAllThread();    
}


/**	执行线程池内的单个线程的函数

	@param	int nIndex - 要执行的线程的编号	
	@return	void - 无	
*/
void CThreadPool::ExecThread(int nIndex)
{
    if ( nIndex < m_nThreadCount )
    {
        MCapi_ResumeThread(m_pThreadArray[nIndex]);
    }
    return;
}

/**	等待线程池内所有线程执行完的函数

	@return	void - 无	
*/
void CThreadPool::WaitAllThread()
{
    int i;

    for ( i = 0; i < m_nThreadCount; i++ )
    {
        MCapi_WaitThread(m_pThreadArray[i], INFINITE);
    }

    for ( i = 0; i < m_nSingleThreadCount; i++ )
    {
        MCapi_WaitThread(m_SingleThread[i], INFINITE);
    }
}
