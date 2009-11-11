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
 *	CapiCommon.h
 *
 *	DESCRIPTION
 *		Interface to a common functions.
 *
 *	HISTORY
 *		10-02-2007	create by zhouweiming.
 *
 */
#ifndef __CAPICOMMON_H__
#define __CAPICOMMON_H__

#ifdef __cplusplus
extern "C" {
#endif


#define CACHE_LINE_SIZE     64

int power2(int n);

int BFind(void **ppData, UINT uCount, void *pData, COMPAREFUNC CompareFunc);

int dtn(int n, int min_n);
int dtn2(int n, int min_n);


void *GetCacheAlignedAddr(void *pAddr);
void *GetNextCacheAlignedAddr(void *pAddr);

#ifndef _WIN32
char* itoa(int value, char *psz, int radix); 
char *strupr(char *s);
int stricmp(char *psz1, char *psz2);
#endif

INT BinCompare( void *str1, int str1_len, void *str2, int str2_len );
INT StrCompare( void *pStr1, void *pStr2 );

INT IntCompare(void *p1, void *p2);
INT PointerCompare(void *p1, void *p2);


#ifdef __cplusplus
}
#endif

#endif //__CAPICOMMON_H__

