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
 *	CNestTaskScheduler.h
 *
 * 	DESCRIPTION
 *		Module for Task Scheduler class 
 * 
 *	HISTORY
 *		06-08-2008	create by zhouweiming.
 *
 */
#ifndef __CWAITTASKSCHEDULER_H__
#define __CWAITTASKSCHEDULER_H__

#include "CLocalQueue.h"
#include "CStealQueue.h"
#include "CDistributedQueue.h"
#include "CThreadPool.h"

class CWaitTask {
public:
    THREADFUNC      func;         //����ִ�к���
    void            *pArg;        //�������Ĳ���
    volatile LONG   *pCount;      //���������ָ�룬Ϊ�ȴ�һ������ִ��������

    CWaitTask();
};

class CTaskArray {
public:
    CWaitTask   *  m_pTaskArray;
    int            m_nTaskArraySize;
    LONG           m_lTaskCount;
    volatile LONG  m_lCurTaskCount; //���������������������
                                 //��������Ϊ�ȴ�һ������ִ��������ж�

    CTaskArray(int nTaskArraySize);
    ~CTaskArray();
    
    INT AddTask(THREADFUNC func, void *pArg);

    LONG GetTaskCount();
    LONG GetCurTaskCount();


};



class CWaitTaskScheduler {
private:
    CThreadPool     m_ThreadPool;//(TaskScheduler_StartFunc, NULL, 0);
    CDistributedQueue<CWaitTask, CLocalQueue<CWaitTask>, 
        CStealQueue<CWaitTask> > m_DQueue;
    THREADFUNC      m_StartFunc;  //Ϊ�̳߳�ʹ�õ��߳���ں���ָ��
    LONG  volatile  m_lTaskId;    //Task Id,�����ж��Ƿ��Ѷ�Ӧ���߳�

public:
    CWaitTaskScheduler();
    virtual ~CWaitTaskScheduler(){};


    //������������Ϊ����������ֱ��ʹ��
    void SetStartFunc(THREADFUNC StartFunc);
    int GetTask(CWaitTask &Task);
    CThreadPool & GetThreadPool();
    LONG AtomicIncrementTaskId();
    
    //������������Ϊ��������ʹ����ʹ��
    void SpawnLocalTask(CWaitTask &Task);
    void SpawnTask(CWaitTask &Task);
    void SpawnTaskArrayAndWait(CTaskArray *pTaskArray);
    void BeginRootThread(CWaitTask &Task);
};

#endif //__CWAITTASKSCHEDULER_H__
