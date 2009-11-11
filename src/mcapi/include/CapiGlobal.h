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
#include <memory.h>
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


extern const int g_ncore;

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


#define CAPI_EXIT_TASK      1
#define CAPI_NOT_EXIT_TASK  0


/* memory allocation functions */
void *CapiMalloc(size_t size);
void *CapiRealloc(void *p, size_t size);
void CapiFree(void *p);


/*** external functions ***/
UINT HashInt( void *pKey, UINT uBucketCount );

UINT HashString( void *pStr, UINT uBucketCount );
UINT HashBin( void *pData, UINT uLength, UINT uBucketCount );


UINT GetStrKeyNoCase( void *pszData, UINT uKeyIndex );
UINT GetStrKey( void *pszData, UINT uKeyIndex );
UINT GetIntKey( void *pData, UINT uKeyIndex );
UINT GetWordKey( UINT uData, UINT uKeyIndex );

#include "Atomic.h"
#include "CapiCommon.h"
#include "mcapi_threads.h"

#ifdef __cplusplus
}
#endif

#endif /* __CAPIGLOBAL_H__ */
