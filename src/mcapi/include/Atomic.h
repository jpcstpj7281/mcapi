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
 *	ApiHook.h
 *
 *	DESCRIPTION
 *		Interface for  atomic operation.
 *
 *	HISTORY
 *		11-11-2009	create by zhouweiming.
 *
 */
#ifndef __ATOMIC_H__
#define __ATOMIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)

BOOL AtomicCAS(LONG volatile *dest, LONG newvalue, LONG oldvalue);
#define AtomicIncrement(x)  InterlockedIncrement(x)
#define AtomicDecrement(x)  InterlockedDecrement(x)
#define AtomicWrite(x, y)     InterlockedExchange(x, y)

BOOL AtomicCAS64(LONGLONG volatile *dest, LONGLONG newvalue, LONGLONG oldvalue);
#define AtomicIncrement64(x)  InterlockedIncrement64(x)
#define AtomicDecrement64(x)  InterlockedDecrement64(x)
#define AtomicWrite64(x, y)   InterlockedExchange64(x, y)

#else

int AtomicCAS(LONG volatile *dest, LONG newvalue, LONG oldvalue);

LONG AtomicWrite(LONG volatile *Target, LONG Value);
LONG AtomicIncrement(LONG volatile *Target);
LONG AtomicDecrement(LONG volatile *Target);
int64_t AtomicCAS64(volatile void *dest, int64_t newvalue, int64_t oldvalue );

#define CAPI_Yield()        sched_yield()

#define         INFINITE                  -1


HANDLE EventCreate();
void WaitEvent(HANDLE hEvent);
void SendEvent(HANDLE hEvent); 
void EventClose(HANDLE hEvent);

HANDLE SemaCreate(int nInitCount, int nMaxCount);
void SemaWait(HANDLE hSem);
void SemaRelease(HANDLE hSem, int nIncCount);
void SemaClose(HANDLE hSem);

#endif /* _WIN32 */


BOOL TAS(LONG volatile *value);

#ifdef __cplusplus
}
#endif

#endif  /* __ATOMIC_H__ */