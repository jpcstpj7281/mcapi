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
#define _WIN32_WINNT 0x0401

#include <windows.h>
#include "CapiGlobal.h"
#include "CThreadPool.h"
#include "CLocalQueue.h"
#include "CStealQueue.h"
#include "CDistributedQueue.h"
#include "CWaitTaskScheduler.h"

#ifdef _WIN32
static unsigned int WINAPI WaitTaskScheduler_StartFunc(void *pArgs);
#else
static void * WaitTaskScheduler_StartFunc(void *pArgs);
#endif

#define     WAITTASK_QUEUE_SIZE     128

CWaitTask::CWaitTask()
{
    pCount = NULL;
}


CTaskArray::CTaskArray(int nTaskArraySize) 
{
    m_nTaskArraySize = nTaskArraySize;
    m_lTaskCount = 0;
    m_lCurTaskCount = 0;

    m_pTaskArray = new CWaitTask[nTaskArraySize];
    INT i;
    for ( i = 0; i < nTaskArraySize; i++ )
    {
        m_pTaskArray[i].pCount = &m_lCurTaskCount;
    }
}


CTaskArray::~CTaskArray() 
{

    delete [] m_pTaskArray;
}



INT CTaskArray::AddTask(THREADFUNC func, void *pArg)
{
    if ( m_lTaskCount >= m_nTaskArraySize )
    {
        return CAPI_FAILED;
    }
    m_pTaskArray[m_lTaskCount].func = func;
    m_pTaskArray[m_lTaskCount].pArg = pArg;
    m_lTaskCount += 1;
    m_lCurTaskCount = m_lTaskCount;

    return CAPI_SUCCESS;
}

LONG CTaskArray::GetTaskCount()
{
    return m_lTaskCount;
}

LONG CTaskArray::GetCurTaskCount()
{
    return m_lCurTaskCount;
}

/**	�ȴ�������ȵĹ��캯��

	@return	constructor - ��	
*/
CWaitTaskScheduler::CWaitTaskScheduler()
{
    m_StartFunc = WaitTaskScheduler_StartFunc;

    int n = m_ThreadPool.GetThreadCount();
    m_DQueue.Create(WAITTASK_QUEUE_SIZE, n, WAITTASK_QUEUE_SIZE, 0, 
        ThreadPool_GetThreadId, &m_ThreadPool);
}

/**	�ȴ�������ȵ������̳߳ص���ں���

	@param	THREADFUNC StartFunc - �̳߳ص���ں���	
	@return	void - ��	
*/
void CWaitTaskScheduler::SetStartFunc(THREADFUNC StartFunc)
{
    m_StartFunc = StartFunc;
}

/**	�ȴ�������ȵĻ�ȡ������

	@param	TASK &Task - ���մӷֲ�ʽ�����л�ȡ������	
	@return	int - �ɹ�����CAPI_SUCCESS, ʧ�ܷ���CAPI_FAILED.	
*/
int CWaitTaskScheduler::GetTask(CWaitTask &Task) 
{
    //�ȴӱ��ض��л�ȡ����
    //���ػ�ȡ����ʧ�ܺ�ӹ�����л�ȡ����
    return m_DQueue.DeQueue(Task);
};


/**	�ȴ�������ȵĻ�ȡ�̳߳غ���

    @return	CThreadPool & - �����̳߳ض���	
*/
CThreadPool & CWaitTaskScheduler::GetThreadPool()
{
    return m_ThreadPool;
}

/**	�ȴ�������ȵ�ԭ����������Id����

    @return	int - ����ԭ�Ӽ�1�������Id.	
*/
LONG CWaitTaskScheduler::AtomicIncrementTaskId()
{
    LONG Id = AtomicIncrement(&m_lTaskId);
    return Id;
}


/**	�ȴ�������ȵ����ɵ�ǰ�̵߳ı�������
    ���񱻷��뵱ǰ�̵߳ı��ض�����

	@param	TASK &Task - ��ִ�е�����	
	@return	void - ��	
*/
void CWaitTaskScheduler::SpawnLocalTask(CWaitTask &Task) 
{
    //��������뱾�ض�����
    m_DQueue.PushToLocalQueue(Task);
};


/**	�ȴ�������ȵ�����������
    ���ɵ����񱻷���ֲ�ʽ������

	@param	TASK &Task - ��ִ�е�����	
	@return	void - ��	
*/
void CWaitTaskScheduler::SpawnTask(CWaitTask &Task) 
{
    if ( m_lTaskId < m_ThreadPool.GetThreadCount() )
    {
        //���λ��Ѹ���������߳�
        LONG Id = AtomicIncrement(&m_lTaskId);
        if ( Id < m_ThreadPool.GetThreadCount() )
        {
            //����֮���Կ��Զ������̵߳ı��ض��н�����ͬ���Ĳ���������Ϊ
            // ������Щ���е��߳��ڽ��Ӳ���֮��ſ�ʼ����
            m_DQueue.PushToLocalQueue(Task, Id);
            m_ThreadPool.ExecThread(Id);
        }
        else
        {
            m_DQueue.EnQueue(Task);
        }
    }
    else
    {
        //���жϹ�������Ƿ��������δ������빲�������
        //�����������뱾�ض�����
        m_DQueue.EnQueue(Task);
    }
};


/**	�ȴ�������ȵ��������̺߳���

	@param	TASK &Task - Ҫִ�е��������	
	@return	void - ��	
*/
void CWaitTaskScheduler::BeginRootThread(CWaitTask &Task)
{
    m_lTaskId = 0;

    m_ThreadPool.CreateThreadPool(m_StartFunc, this, 0);

    m_DQueue.PushToLocalQueue(Task, 0);

    m_ThreadPool.ExecThread( 0 );  

    m_ThreadPool.WaitAllThread();
}

/**	�ȴ�������ȵ��̳߳���ں���

	@param	void *pArgs - CWaitTaskScheduler���͵Ĳ���	
	@return	unsigned int WINAPI - ����0	
*/
#ifdef _WIN32
static unsigned int WINAPI WaitTaskScheduler_StartFunc(void *pArgs)
#else
static void * WaitTaskScheduler_StartFunc(void *pArgs)
#endif
{
    CWaitTaskScheduler  *pSched = (CWaitTaskScheduler *)pArgs;

    CWaitTask    Task;
    int     nRet;

    for ( ;; )
    {
        nRet = pSched->GetTask(Task);

        if ( nRet == CAPI_FAILED )
        {
            CThreadPool &ThreadPool = pSched->GetThreadPool();
            
            // ����һ��������߳�,��ֹ��������С��CPU����ʱ��
            // ��Ȼ�������ڹ���״̬,�Ӷ�����WaitAllThread()��������״̬
            // ������ѹ�����һ�����еĹ��̣������ѵ�������������һ�������߳�
            LONG Id = pSched->AtomicIncrementTaskId();
            if ( Id < ThreadPool.GetThreadCount() )
            {
                ThreadPool.ExecThread(Id);
            }
            break;
        }

        (*(Task.func))(Task.pArg);

        if ( Task.pCount != NULL )
        {
            AtomicDecrement(Task.pCount);
        }
    }

    return 0;
}


void CWaitTaskScheduler::SpawnTaskArrayAndWait(CTaskArray *pTaskArray)
{
    int i;
    for (i = 0; i < pTaskArray->GetTaskCount(); i++ )
    {
        SpawnTask(pTaskArray->m_pTaskArray[i]);
    }

    // �ȴ�����������б�ȫ��ִ�����
    while ( pTaskArray->GetCurTaskCount() != 0 )
    {
        //�л�������������ִ��
        CWaitTask task;
        int nRet = GetTask(task);

        if ( nRet == CAPI_FAILED )
        {
            CAPI_Yield();
        }
        else
        {
            (*(task.func))(task.pArg);

            if ( task.pCount != NULL )
            {
                AtomicDecrement(task.pCount);
            } 
        }
    }
    //pTaskArray->GetCurTaskCount()����0��ʾ������Ҫ�ȴ�������ִ�����
    //��ʱ�����˳��ȴ���
    return;
}



