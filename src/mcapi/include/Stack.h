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
 *	stack.h
 *
 *	DESCRIPTION
 *		Interface to a generic stack object.
 *
 *	HISTORY
 *		09-21-98	create by zhouweiming.
 *
 */

#ifndef __STACK_H__
#define __STACK_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct STACK_st {
	void		**ppBase;    /* ������¼�����������ݵ����� */
	UINT		uTop;        /* ջ��λ�� */
	unsigned	uStackSize;  /* ջ�����ߴ磬Ҳ��������Ĵ�С */
} STACK;


STACK *	Stack_Create(UINT uStackSize);
void	Stack_Destroy(STACK * pStack, DESTROYFUNC DestroyFunc);
void *	Stack_Pop( STACK * pStack );
INT     Stack_Push( STACK * pStack, void *pData );
INT     Stack_IsEmpty(STACK *pStack);    

#ifdef __cplusplus
}
#endif

#endif /* __STACK_H__ */