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
 *	CapiGlobal.h
 *
 *	DESCRIPTION
 *		global Prototypes and definition for capi library.
 *
 *	HISTORY
 *		10-03-2003	create by zhouweiming.
 *
 */
#ifndef __CAPIGLOBAL_H__
#define __CAPIGLOBAL_H__

#if _WIN32_WINNT < 0x0404
#undef _WIN32_WINNT
#define _WIN32_WINNT  0x0404
#endif

#ifdef _WIN32 
#include <windows.h>
#else
#include <unistd.h>
#endif
//#include <stdlib.h>
#include <memory.h>
//#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _TEST 
#define PRIVATE public
#else
#define PRIVATE private
#endif

#define CAPI_SUCCESS        0
#define CAPI_FAILED         1
#define CAPI_NO_MEMORY      2
#define CAPI_NOT_FOUND      3


typedef int             BOOL;
typedef long int        LONG;
typedef unsigned long   DWORD;
typedef unsigned int    UINT;
typedef int             INT;
typedef void *          HANDLE;

#ifndef _WIN32 
#define _stdcall
#define __stdcall
#define WINAPI
typedef long long int LONGLONG;
typedef long long int int64_t;
#define Sleep(x)      usleep(x)

#define  TRUE         1
#define  FALSE        0

#endif


/* ��������Ľڵ�ṹ */
typedef struct SINGLENODE_st {
	struct SINGLENODE_st *pNext;     /* ��һ���ڵ�ָ�� */
    void	*pData;                  /* ����ָ�� */
} SINGLENODE, *PSINGLENODE;

typedef struct DOUBLENODE_st {
    struct DOUBLENODE_st *pNext;     /* ��һ���ڵ�ָ�� */
    struct DOUBLENODE_st *pPrev;     /* ǰһ���ڵ�ָ�� */
    void    *pData;                  /* ����ָ�� */
} DOUBLENODE;

#ifdef _WIN32
typedef unsigned int (_stdcall *THREADFUNC)(void *pArgs);
#else
typedef unsigned int (*THREADFUNC)(void *pArgs);
#endif

typedef struct  TASK_st {
    THREADFUNC      func;         //����ִ�к���
    void            *pArg;        //�������Ĳ���
    DWORD           time_consume; //����ִ�еĺ�ʱ
} TASK;


/**	ͨ���������ݱȽϺ���

	@param	void *pData1 -Ҫ�Ƚϵĵ�1������	
	@param	void *pData2 -Ҫ�Ƚϵĵ�2������	
        @return	INT - С��0��ʾpData1С��pData2, ����0��ʾpData1����pData2,
                  ����0��ʾpData1����pData2
*/
typedef INT (*COMPAREFUNC) ( void *pData1, void *pData2 );

/**	ͨ�����������ͷź���

	@param	void *pData -Ҫ�ͷŵ�����	
	@return	 -��	
*/
typedef void (*DESTROYFUNC) ( void *pData );

/**	ͨ���������ݵı���ִ�к���

	@param	void *pData - Ҫ����������ָ��	
	@return	void - ��	
*/
typedef void (*TRAVERSEFUNC)( void *pData );


/**	ͨ���������ݵı���ִ�к���

	@param	void *pData - Ҫ����������ָ��	
	@return	void - ��	
*/
typedef INT (*VISITFUNC)(void *pData);

/**	ͨ�����ݿ�������

	@param	void *pData - Ҫ����������	
	@return	void * - �ɹ����ؿ��������ݣ�ʧ�ܷ���NULL	
*/
typedef void *(*COPYFUNC)(void *pData);

/**	��������Ļ�ȡ�ؼ���ת������ŵĺ���

	@param	void *pData - �ؼ���ָ��	
    @param	UINT *uKeyIndex - �ؼ��ֵ�λ��	
	@return	UINT - �ؼ���ת��������	
*/
typedef UINT (*GETKEYFUNC)( void *pData, UINT uKeyIndex );

/**	�����ϣֵ�Ļص�����

        @param  void *pKey - Ҫ�����ϣֵ�Ĺؼ���	
        @param  UINT uBucketNum - ��ϣ���е�BUCKET�����С	
	@return UINT - ���ؼ�����Ĺ�ϣֵ��һ��������ϣ��BUCKET�����±ꡣ	
*/
typedef UINT (*HASHFUNC)(void *pKey, UINT uBucketNum);

#define     EVENT           HANDLE
#define     SEMAPHORE       HANDLE

#define         MCAPI_THREAD_RUNNING      1
#define         MCAPI_THREAD_EXIT         2
#define         MCAPI_THREAD_SUSPEND      3

HANDLE MCapi_CreateThread(THREADFUNC func, void *args, INT nFlag);
void MCapi_WaitThread(HANDLE hThread, INT nMilliSeconds);
void MCapi_SuspendThread(HANDLE hThread);
void MCapi_ResumeThread(HANDLE hThread);
void MCapi_CloseThread(HANDLE hThread);

#if defined(_WIN32)

//#define     LOCK            HANDLE

//#define LockCreate()        CreateMutex(NULL, FALSE, NULL)
//#define Lock(x)             (void)WaitForSingleObject((x), INFINITE)
//#define Unlock(x)           (void)ReleaseMutex(x)
//#define LockClose(x)        (void)CloseHandle(x)

#define EventCreate()       CreateEvent(NULL, TRUE, FALSE, NULL)
#define WaitEvent(x)        (void)WaitForSingleObject((x), INFINITE)
#define SendEvent(x)        (void)SetEvent(x)
#define EventClose(x)       (void)CloseHandle(x)


#define SemaCreate(x,y)     CreateSemaphore(NULL,x,y,NULL)
#define SemaWait(x)         WaitForSingleObject(x,INFINITE)
#define SemaRelease(x,y)    ReleaseSemaphore(x,y,NULL)
#define SemaClose(x)        CloseHandle(x)

#define CAPI_Yield()        SwitchToThread()

BOOL AtomicCAS(LONG volatile *dest, LONG newvalue, LONG oldvalue);

#define AtomicIncrement(x)  InterlockedIncrement(x)
#define AtomicDecrement(x)  InterlockedDecrement(x)
#define AtomicWrite(x, y)     InterlockedExchange(x, y)

BOOL AtomicCAS64(LONGLONG volatile *dest, LONGLONG newvalue, LONGLONG oldvalue);

#define AtomicIncrement64(x)  InterlockedIncrement64(x)
#define AtomicDecrement64(x)  InterlockedDecrement64(x)
#define AtomicWrite64(x, y)   InterlockedExchange64(x, y)

#else

int AtomicCAS(unsigned long *mem,unsigned long newval,unsigned long oldval) 

/* BOOL AtomicCAS(volatile void *ptr, int value, int comparand); */
LONG AtomicWrite(LONG volatile *Target, LONG Value);
LONG AtomicIncrement(LONG volatile *Target);
LONG AtomicDecrement(LONG volatile *Target);
int64_t AtomicCAS64(volatile void *ptr, int64_t value, int64_t comparand );

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


#ifdef _WIN32
#define    TlsGetValueFunc   TlsGetValue
#define    TlsSetValueFunc   TlsSetValue
#else
#define    TlsGetValueFunc   pthread_getspecific
#define    TlsSetValueFunc   pthread_setspecific
#define    TlsFree(x)        pthread_key_delete(x)
#endif

    

#define    CAPI_EXIT_TASK      1
#define    CAPI_NOT_EXIT_TASK  0


/* memory allocation functions */
void *CapiMalloc(size_t size);
void *CapiRealloc(void *p, size_t size);
void CapiFree(void *p);


/*** external functions ***/
UINT        HashInt( void *pKey, UINT uBucketCount );
INT         StrCompare( void *pStr1, void *pStr2 );
INT         PointerCompare(void *p1, void *p2);
INT         IntCompare(void *pInt1, void *pInt2);
INT         BinCompare( void *str1, int str1_len, void *str2, int str2_len );

UINT        HashString( void *pStr, UINT uBucketCount );
UINT        HashBin( void *pData, UINT uLength, UINT uBucketCount );


UINT GetStrKeyNoCase( void *pszData, UINT uKeyIndex );
UINT GetStrKey( void *pszData, UINT uKeyIndex );
UINT GetIntKey( void *pData, UINT uKeyIndex );
UINT GetWordKey( UINT uData, UINT uKeyIndex );



#ifdef __cplusplus
}
#endif

#endif /* __CAPIGLOBAL_H__ */
