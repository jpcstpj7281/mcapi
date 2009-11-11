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
    THREADFUNC      func;         //任务执行函数
    void            *pArg;        //任务函数的参数
    volatile LONG   *pCount;      //任务组计数指针，为等待一组任务执行完而设计

    CWaitTask();
};

class CTaskArray {
public:
    CWaitTask   *  m_pTaskArray;
    int            m_nTaskArraySize;
    LONG           m_lTaskCount;
    volatile LONG  m_lCurTaskCount; //任务组中任务个数计数，
                                 //可以用来为等待一组任务执行完进行判断

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
    THREADFUNC      m_StartFunc;  //为线程池使用的线程入口函数指针
    LONG  volatile  m_lTaskId;    //Task Id,用于判断是否唤醒对应的线程

public:
    CWaitTaskScheduler();
    virtual ~CWaitTaskScheduler(){};


    //下面两个函数为调度器本身直接使用
    void SetStartFunc(THREADFUNC StartFunc);
    int GetTask(CWaitTask &Task);
    CThreadPool & GetThreadPool();
    LONG AtomicIncrementTaskId();
    
    //下面三个函数为调度器的使用者使用
    void SpawnLocalTask(CWaitTask &Task);
    void SpawnTask(CWaitTask &Task);
    void SpawnTaskArrayAndWait(CTaskArray *pTaskArray);
    void BeginRootThread(CWaitTask &Task);
};

#endif //__CWAITTASKSCHEDULER_H__
