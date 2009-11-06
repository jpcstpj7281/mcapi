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
 *	MTList.h
 *
 *	DESCRIPTION
 *		Interface to a generic multiple task list object.
 *      使用一个锁来保护枚举的情况，
        添加和删除操作如果和当前枚举节点相关的话需要
        修改枚举变量当前节点指针
 *	HISTORY
 *		09-12-2000	create by zhouweiming.
 *
 */
#ifndef __MTLIST_H__
#define __MTLIST_H__

#ifdef __cplusplus
extern "C" {
#endif


#define     MTLIST_DELETE_NODE      0
#define     MTLIST_NO_DELETE        1



typedef struct MTLIST_st {
    DOUBLELIST  *pList;    /* 双向链表指针 */
    MTASK      *pMTask;    /* 多任务退出结构指针 */
} MTLIST;


MTLIST *MTList_Create(void);
void    MTList_Destroy(MTLIST *pList, DESTROYFUNC DestroyFunc);

UINT    MTList_GetExitFlag(MTLIST *pList);
void    MTList_SetExitFlag(MTLIST *pList, UINT uExitFlag);
void    MTList_EnterTask(MTLIST *pList);
void    MTList_LeaveTask(MTLIST *pList);

INT     MTList_InsertHead(MTLIST *pList, void *pData);
INT     MTList_DeleteHead(MTLIST *pList, DESTROYFUNC DestroyFunc);
INT     MTList_InsertTail(MTLIST *pList, void *pData);
void *  MTList_PopHead(MTLIST *pList);
void *  MTList_PopTail(MTLIST *pList);


void    MTList_EnumBegin(MTLIST *pList);
INT     MTList_EnumNext(MTLIST *pList, VISITFUNC VisitFunc);
INT     MTList_EnumNextCopy(MTLIST *pList, VISITFUNC VisitFunc,
                        DESTROYFUNC DestroyFunc, COPYFUNC CopyFunc);
INT     MTList_EnumNextAndDelete(MTLIST *pList, VISITFUNC VisitFunc,
                             DESTROYFUNC DestroyFunc);

#ifdef __cplusplus
}
#endif

#endif /* __MTLIST_H__ */
