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
*	CDistributedQueue.h
*
*	DESCRIPTION
*		Module for Distributed Queue class 
*
*	HISTORY
*		06-08-2008	create by zhouweiming.
*
*/
#ifndef __CDISTRIBUTEDQUEUE_H__
#define __CDISTRIBUTEDQUEUE_H__

#if _DEBUG 
#include <stdio.h>
#endif
#ifndef _WIN32
#include <pthread.h>
#endif
#include <stdlib.h>
#include <omp.h>
#include "CapiGlobal.h"
#include "FastLock.h"
#include "CScopedLock.h"

//��ȡ�߳�Id�ص���������
typedef int (*GetThreadIdFunc)(void *pArg); 

template <class T, class LocalQueue, class SharedQueue, class SubQueue> 
class CDistributedQueue {
private:
    LocalQueue **    m_ppLocalQueue;    // ���ض�������
    SharedQueue *   m_pSharedQueue;     // ������гػ������
  
    int             m_nLocalQueueSize;
    int             m_nSharedQueueSize;
    int             m_nLocalQueueCount;
    int             m_nSharedQueueCount; 
#ifdef _WIN32
    DWORD           m_dwTlsIndex;        //�̱߳��ش洢����
#else
    pthread_key_t   m_dwTlsIndex;
#endif
    LONG volatile   m_lThreadIdIndex;    //�̱߳�����ֵ
    GetThreadIdFunc m_GetThreadIdFunc;   //��ȡ�̱߳�Żص�����ָ�룬���������
										 //���̳߳��ṩ���ʱ����Ҫ����ص�����
    void *          m_pThreadIdFuncArg;  //��ȡ�̱߳�Żص������Ĳ���

    CFastLock       m_LocalQueueResizeLock; //רΪ�����ResizeLocalQueue����ʹ��
    void ResizeLocalQueue();             //��m_ppLocalQueue����Ĵ�С����һ��
public:

    CDistributedQueue(){
        m_GetThreadIdFunc = NULL;
        m_pThreadIdFuncArg = NULL;
        m_lThreadIdIndex = 0;
        m_ppLocalQueue = NULL;
        m_pSharedQueue = NULL;
    };
    void Create( int nLocalQueueSize, int nLocalQueueCount, 
        int nSharedQueueSize, int nSharedQueueCount); 
    void Create( int nLocalQueueSize, int nLocalQueueCount, 
        int nSharedQueueSize, int nSharedQueueCount,
        GetThreadIdFunc GetThreadId, void * pThreadIdFuncArg);

    virtual ~CDistributedQueue();

    LONG ThreadIdGet();

    void EnQueue(T &Data);
    int  DeQueue(T &Data);

    void PushToLocalQueue(T &Data);
    void PushToLocalQueue(T &Data, int nIndex);
    int PopFromLocalQueue(T &Data);

    int PushToSharedQueue(T &Data);
    int PopFromSharedQueue(T &Data);

    SharedQueue *GetSharedQueue() { return m_pSharedQueue; };
    int PrivatizeSharedQueue(int nSharedQueueIndex);
};



/**	�ֲ�ʽ���еĴ�������

	@param	int nLocalQueueSize - �����Ӷ��еĴ�С	
	@param	int nLocalQueueCount - ���ض��еĸ���(����Ĵ�С)
                                   Ϊ0��ʾ��CPU������ȵĸ���
	@param	int nSharedQueueSize - �����Ӷ��еĴ�С	
	@param	int nSharedQueueCount - �����Ӷ��еĸ���	
                                    Ϊ0��ʾ��CPU������ȵĸ���
	@return	void - �� 
*/
template <class T, class LocalQueue, class SharedQueue, class SubQueue> 
void CDistributedQueue<T, LocalQueue, SharedQueue, SubQueue>::Create(
                   int nLocalQueueSize, int nLocalQueueCount, 
                   int nSharedQueueSize, int nSharedQueueCount) 
{
    m_nLocalQueueSize = nLocalQueueSize;
    m_nSharedQueueSize = nSharedQueueSize;
    if ( nLocalQueueCount != 0 )
    {
        m_nLocalQueueCount = nLocalQueueCount;
    }
    else
    {
        m_nLocalQueueCount = omp_get_num_procs();
    }

    if ( nSharedQueueCount != 0 )
    {
        m_nSharedQueueCount = nSharedQueueCount;
    }
    else
    {
        m_nSharedQueueCount = omp_get_num_procs();
    }

    m_ppLocalQueue = new LocalQueue *[m_nLocalQueueCount];
    int i;
    for ( i = 0; i < m_nLocalQueueCount; i++ )
    {
        m_ppLocalQueue[i] = NULL;
    }
    m_pSharedQueue = new SharedQueue(m_nSharedQueueCount, m_nSharedQueueSize);
#ifdef _WIN32
    m_dwTlsIndex = TlsAlloc();
#else
    pthread_key_create( &m_dwTlsIndex, NULL );
#endif

    m_lThreadIdIndex = 0;
}


/**	�ֲ�ʽ���еĴ�������

	@param	int nLocalQueueSize - �����Ӷ��еĴ�С	
	@param	int nLocalQueueCount - ���ض��еĸ���(����Ĵ�С)	
	@param	int nSharedQueueSize - �����Ӷ��еĴ�С	
	@param	int nSharedQueueCount - �����Ӷ��еĸ���	
	@param	GetThreadIdFunc GetThreadId - ��ȡ�߳�Id�ص�����	
	@param	void * pThreadIdFuncArg - GetThreadId�ص������Ĳ���	
	@return	void - ��	
*/
template <class T, class LocalQueue, class SharedQueue, class SubQueue> 
void CDistributedQueue<T, LocalQueue, SharedQueue, SubQueue>::Create(
    int nLocalQueueSize, int nLocalQueueCount, 
    int nSharedQueueSize, int nSharedQueueCount,
    GetThreadIdFunc GetThreadId, void * pThreadIdFuncArg)
{
    m_GetThreadIdFunc = GetThreadId;
    m_pThreadIdFuncArg = pThreadIdFuncArg;
    Create(nLocalQueueSize, nLocalQueueCount, nSharedQueueSize, nSharedQueueCount);
}



/**	�ֲ�ʽ���е���������

	@return	 - ��	
*/
template <class T, class LocalQueue, class SharedQueue, class SubQueue> 
CDistributedQueue<T, LocalQueue, SharedQueue, SubQueue>::~CDistributedQueue()
{
    int i;
#if 1
    if ( m_ppLocalQueue != NULL )
    {
#if 1
        for ( i = 0; i < m_nLocalQueueCount; i++ )
        {
            if ( m_ppLocalQueue[i] != NULL )
            {
                delete m_ppLocalQueue[i];
            }
        }
#endif
        delete [] m_ppLocalQueue;

    }
#endif
    if ( m_pSharedQueue != NULL )
    {
        delete m_pSharedQueue;
    }
#ifdef _WIN32
    TlsFree(m_dwTlsIndex);
#else
    pthread_key_delete(m_dwTlsIndex);
#endif
}

/**	�ֲ�ʽ���еĽ����ض�����������һ�����ڲ���Ա����

	@return	void - ��	
*/
template <class T, class LocalQueue, class SharedQueue, class SubQueue> 
void CDistributedQueue<T, LocalQueue, SharedQueue, SubQueue>::ResizeLocalQueue()
{
    //�����ض�����������һ��, ��ֹ�߳��������ڶ�������,�Ա�֤����ȫ
    int i;

    LocalQueue **ppQueue = new LocalQueue *[m_nLocalQueueCount * 2];
    for ( i = 0; i < m_nLocalQueueCount; i++ )
    {
        ppQueue[i] = m_ppLocalQueue[i];
    }
    for ( i = m_nLocalQueueCount; i < m_nLocalQueueCount * 2; i++ )
    {
        ppQueue[i] = NULL;
    }
    free(m_ppLocalQueue);
    m_ppLocalQueue = ppQueue;

    //ʹ��ԭ�Ӳ�������m_nLocalQueueCount�����ݾ�������
    AtomicWrite((LONG volatile *)&m_nLocalQueueCount, m_nLocalQueueCount * 2);

}

/**	�ֲ�ʽ���еĻ�ȡ�߳�Id����
	���m_GetThreadIdFunc�ص�������Ϊ�գ���ʹ������ȡId
	������ݷֲ�ʽ�����ڲ��ı�Ż��ƻ�ȡ�߳�Id

	@return	LONG - �����̵߳ı��	
*/
template <class T, class LocalQueue, class SharedQueue, class SubQueue> 
LONG CDistributedQueue<T, LocalQueue, SharedQueue, SubQueue>::ThreadIdGet()
{
    LONG Id;
    LocalQueue *pQueue = NULL;

    if ( m_GetThreadIdFunc != NULL )
    {
        Id = (*m_GetThreadIdFunc)(m_pThreadIdFuncArg);
        if ( Id >= m_nLocalQueueCount )
        {
            CScopedLock<CFastLock> slock(m_LocalQueueResizeLock);
            if ( Id >= m_nLocalQueueCount )
            {
                ResizeLocalQueue();
            }
        }
        if ( m_ppLocalQueue[Id] == NULL )
        {
            m_ppLocalQueue[Id] = new LocalQueue(m_nLocalQueueSize);
        }
        return Id;
    }
    else
    {
        Id = (LONG )TlsGetValueFunc(m_dwTlsIndex);
        if ( Id == 0 )
        {
            Id = AtomicIncrement(&m_lThreadIdIndex);
            TlsSetValueFunc(m_dwTlsIndex, (void *)Id);
            pQueue = new LocalQueue(m_nLocalQueueSize);
        }
        --Id;
    }

    if ( Id >= m_nLocalQueueCount)
    {
        CScopedLock<CFastLock> slock(m_LocalQueueResizeLock);
        if ( Id >= m_nLocalQueueCount )
        {
            ResizeLocalQueue();
        }
    }
    if ( pQueue != NULL )
    {
        m_ppLocalQueue[Id] = pQueue;
    }
    return Id;
}


/**	�ֲ�ʽ���еĽ��Ӳ�������
	����ٶ��˱��ض��п������޽���
	���Ӳ��԰��������ȼ����У�
	1�����ض��п�ʱ���뱾�ض��У�2���������δ��ʱ���빲�����
	3�����������ʱ���뱾�ض���

	@param	T &Data - Ҫ���ӵ�����	
	@return	void - ��	
*/
template <class T, class LocalQueue, class SharedQueue, class SubQueue> 
void CDistributedQueue<T, LocalQueue, SharedQueue, SubQueue>::EnQueue(T &Data)
{
    int nId = ThreadIdGet();

    if ( m_ppLocalQueue[nId]->IsEmpty() )
    {
        m_ppLocalQueue[nId]->EnQueue(Data);
    }
    else if ( m_pSharedQueue->Push(Data) != CAPI_SUCCESS )
    {
        int nId = ThreadIdGet();
        m_ppLocalQueue[nId]->EnQueue(Data);
    }
    else
    {
        //�����֧����Ҫ���κ���
    }
    return;
}

/**	�ֲ�ʽ���еı��ض��н��Ӻ���
	�����ݽ��뵽��ǰ�̵߳ı��ض�����

	@param	T &Data - Ҫ���ӵ�����	
	@return	void - ��	
*/
template <class T, class LocalQueue, class SharedQueue, class SubQueue> 
void CDistributedQueue<T, LocalQueue, SharedQueue, SubQueue>::PushToLocalQueue(T &Data)
{
    int nId = ThreadIdGet();
    m_ppLocalQueue[nId]->EnQueue(Data);
    return;
}

/**	�ֲ�ʽ���е�ָ����ű��ض��н��Ӻ���
    ����һ��Ϊ�����������Ƶĺ���
    ʹ���������Ҫ�ر�С�ģ����뱣֤���ᷢ�����ݾ�������

	@param	T &Data - Ҫ���ӵ�����	
	@param	int nIndex - ���ض��е����	
	@return	void - ��	
*/
template <class T, class LocalQueue, class SharedQueue, class SubQueue> 
void CDistributedQueue<T, LocalQueue, SharedQueue, SubQueue>::PushToLocalQueue(T &Data, int nIndex)
{
    if ( nIndex >= m_nLocalQueueCount * 2)
    {
        return;
    }

    if ( nIndex >= m_nLocalQueueCount )
    {
        CScopedLock<CFastLock> slock(m_LocalQueueResizeLock);
        if ( nIndex >= m_nLocalQueueCount )
        {
            ResizeLocalQueue();
        }
    }

    if ( m_ppLocalQueue[nIndex] == NULL )
    {
        m_ppLocalQueue[nIndex] = new LocalQueue(m_nLocalQueueSize);
    }

    m_ppLocalQueue[nIndex]->EnQueue(Data);
    return;
}

/**	�ֲ�ʽ���еı��ض��г��Ӻ���

	@param	T &Data - ���ճ��ӵ�����	
	@return	int - ���ӳɹ�����CAPI_SUCCESS, ʧ��(����Ϊ�գ�����CAPI_FAILED.	
*/
template <class T, class LocalQueue, class SharedQueue, class SubQueue> 
int CDistributedQueue<T, LocalQueue, SharedQueue, SubQueue>::PopFromLocalQueue(T &Data)
{
    int nId = ThreadIdGet();
    return m_ppLocalQueue[nId]->DeQueue(Data);
}

/**	�ֲ�ʽ���еĹ�����н��Ӻ���

	@param	T &Data - Ҫ���ӵ�����	
	@return	int - �ɹ�����CAPI_SUCCESS, ʧ�ܷ���CAPI_FAILED.	
*/
template <class T, class LocalQueue, class SharedQueue, class SubQueue> 
int CDistributedQueue<T, LocalQueue, SharedQueue, SubQueue>::PushToSharedQueue(T &Data)
{
   return m_pSharedQueue->Push(Data);
}

/**	�ֲ�ʽ���еĹ�����г��Ӻ���

	@param	T &Data - ���ճ��ӵ�����	
	@return	int - �ɹ�����CAPI_SUCCESS, ʧ�ܷ���CAPI_FAILED.	
*/
template <class T, class LocalQueue, class SharedQueue, class SubQueue> 
int CDistributedQueue<T, LocalQueue, SharedQueue, SubQueue>::PopFromSharedQueue(T &Data)
{
    return m_pSharedQueue->Pop(Data);
}

/**	�ֲ�ʽ���еĳ��Ӻ���
    ���Ӳ�������Ϊ���ȴӱ��ض����г��ӣ����ʧ����ӹ�������г���

	@param	T &Data - ���ճ��ӵ�����	
	@return	int - �ɹ�����CAPI_SUCCESS, ʧ�ܷ���CAPI_FAILED.	
*/
template <class T, class LocalQueue, class SharedQueue, class SubQueue> 
int CDistributedQueue<T, LocalQueue, SharedQueue, SubQueue>::DeQueue(T &Data)
{
    int nRet;

    int nId = ThreadIdGet();
    
    nRet = m_ppLocalQueue[nId]->DeQueue(Data);
    if ( nRet == CAPI_FAILED )
    {
        nRet = m_pSharedQueue->Pop(Data);
#if _DEBUG 
        if (nRet == CAPI_SUCCESS )
        {
//            printf("Thread Id:%ld, Get Data From SharedQueue\n", GetCurrentThreadId());
        }
#endif
    }
#if _DEBUG 
    else
    {
//        printf("Thread Id:%ld, Get Data From LocalQueue\n", GetCurrentThreadId());
    }
#endif
    return nRet;   
}

/**	�ֲ�ʽ���еĹ����������˽�л�����

	@param	int nSharedQueueIndex - �����Ӷ������	
	@return	int - �ɹ�����CAPI_SUCCESS, ʧ�ܷ���CAPI_FAILED.	
*/
template <class T, class LocalQueue, class SharedQueue, class SubQueue> 
int CDistributedQueue<T, LocalQueue, SharedQueue, SubQueue>::PrivatizeSharedQueue(int nSharedQueueIndex)
{
    int nRet = CAPI_FAILED;
    T   Data;

    int nId = ThreadIdGet();

    SubQueue *pQueue;

    pQueue = m_pSharedQueue->GetSubQueue(nSharedQueueIndex);
    if ( pQueue->IsEmpty() )
    {
        return nRet;
    }

    pQueue->Lock();

    int i;

    int nCount = pQueue->GetCountNoLock();
    if ( nCount >= 2 )
    {
        nCount /= 2;
    }

    for ( i = 0; i < nCount; i++ )
    {
        nRet = pQueue->DeQueueNoLock( Data ); 
        m_ppLocalQueue[nId]->EnQueue(Data);
    }

    pQueue->Unlock();

    return nRet;
}


#endif //__CDISTRIBUTEDQUEUE_H__

