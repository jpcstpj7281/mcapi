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
#include <stdlib.h>
#include <omp.h>
#include "CapiGlobal.h"
#include "CapiCommon.h"


#ifdef _WIN32
#pragma warning(disable : 4996)
#endif


int power2(int n)
{
	if ( n > 32 )
	{
		return -1;
	}
	return 1 << n;
}


/**	���ֲ��Һ��������ô˺���ǰ����Ա��С�����ź���

    @param	void **ppData - ��������ָ��
    @param	UINT uCount - ppData�����е��������ݸ���
    @param	void *pData - Ҫ���ҵ�ƥ������
    @param	COMPAREFUNC CompareFunc - �ȽϺ���
    @return	void * - �ɹ����ز鵽�ľ�ȷ���ݻ�պñ�Ҫ���ҵ����ݴ������,
                     ʧ�ܷ���-1��
*/
int  BFind(void **ppData, UINT uCount, void *pData, COMPAREFUNC CompareFunc)
{
    int nLow;
    int nHigh;
    int nMid;

    if ( ppData == NULL || CompareFunc == NULL || pData == NULL )
    {
        return -1;
    }

    nLow = 0;
    nHigh = uCount - 1;
    nMid = 0;
    while ( nLow <= nHigh )
    {
        INT nResult;
        nMid = ( nLow + nHigh ) / 2;
        nResult = (*CompareFunc)( ppData[nMid], pData );
        if ( nResult > 0 )
        {
            nHigh = nMid - 1;
        }
        else if ( nResult < 0 )
        {
            nLow = nMid + 1;
        }
        else
        {
            /* �Ѿ�������ƥ�����ݣ����� */
            return nMid;
        }
    }
    if ( nMid == nLow )
    {
        return nLow;
    }
    else if ( nMid == nHigh )
    {
        if ( nMid < (int)(uCount - 1))
        {
            return nMid + 1;
        }
        else
        {
            return -1;  //����û�бȴ��������ݴ������
        }
    }
    else
    {
        return nHigh;
    }
}



/**	����ѭ��������Ҫ���߳�����
    ����ѭ������������CPU������һ���߳�������Ҫ��ѭ����������
    ���������Ҫ���߳������������������߳�����������CPU����

    @param	int n - ѭ����������	
    @param	int min_n - �����߳���Ҫ�����ٵ�������	
    @return	int - �߳���������������1����CPU������	
*/
int dtn(int n, int min_n)
{
    int max_tn = n / min_n;
    int tn = max_tn > g_ncore ? g_ncore : max_tn;  //tn��ʾҪ���õ��߳�����
    if ( tn < 1 )
    {
        tn = 1;
    }
    return tn;
}


/**	����ѭ��������Ҫ���߳�����
    ����ѭ������������CPU������һ���߳�������Ҫ��ѭ����������
    ���������Ҫ���߳������������������߳�����������CPU����������

    @param	int n - ѭ����������	
    @param	int min_n - �����߳���Ҫ�����ٵ�������	
    @return	int - �߳���������������2����CPU������	
*/
int dtn2(int n, int min_n)
{
    int max_tn = n / min_n;
    int tn = max_tn > 2*g_ncore ? 2*g_ncore : max_tn;  //tn��ʾҪ���õ��߳�����
    if ( tn < 1 )
    {
        tn = 1;
    }
    return tn;
}



/**	���������ַ֮��ĵ�1��Cache���׵�ַ
    ���������ַ�պ�Ϊһ��Cache���׵�ַ����ô����������Cache���׵�ַ

    @param	void *pAddr - �����ĵ�ַ	
    @return	void * - ���ظ�����ַ֮��ĵ� 1 ��Cache���׵�ַ	
*/
void *GetCacheAlignedAddr(void *pAddr)
{
    int  m = CACHE_LINE_SIZE;

    void *pRet = (void *)(((UINT)pAddr+m-1)&(-m));

    return pRet;
}


/**	���������ַ֮��ĵ�1��Cache���׵�ַ
    ���������ַ�պ�Ϊһ��Cache���׵�ַ����ô����������������һ��Cache���׵�ַ

    @param	void *pAddr - �����ĵ�ַ	
    @return	void * - ���ظ�����ַ֮��ĵ�1��Cache���׵�ַ	
*/
void *GetNextCacheAlignedAddr(void *pAddr)
{
    int  m = CACHE_LINE_SIZE;

    void *pRet = (void *)(((UINT)(pAddr) + m )&(-m));

    return pRet;
}


#ifndef _WIN32
char* itoa(int value, char *psz, int radix)  
{
    if ( psz!= NULL ) 
    {
        sprintf(psz, "%d", value);
    }
    return psz;
}



char *strupr(char *s)
{
    int i, len;

    len = strlen(s);

    for ( i = 0; i < len; i++ )
    {
        if((s[i] >= 'a') && (s[i] <= 'z' ) )
        {
            s[i] = s[i] - 'a' + 'A'; 
        }
    }
    return s;
}

int stricmp(char *psz1, char *psz2)
{
    int  ret;
    char *pTemp1;
    char *pTemp2;

    pTemp1 = malloc(strlen(psz1) + 1);
    pTemp2 = malloc(strlen(psz2) + 1);

    strcpy(pTemp1, psz1);
    strcpy(pTemp2, psz2);

    strupr(pTemp1);
    strupr(pTemp2);

    ret = strcmp(pTemp1, pTemp2);

    free(pTemp1);
    free(pTemp2);

    return ret;
}

#endif


/*
 *	BinCompare( )
 *	Compare if two binary data is equal.
 *	Return Values:
 *		0		equal
 *		1		first > second 
 *      -1      first < second
 */
INT BinCompare( void *str1, int str1_len, void *str2, int str2_len )
{
	char	*psz1, *psz2;
	int		i;
    int     len;

    len = str1_len < str2_len ? str1_len:str2_len;

	psz1 = (char *)str1;
	psz2 = (char *)str2;

	i = 0;
	while ( i < len )
	{
		if ( *psz1 > *psz2 )
		{
			return 1;
		}
        else if ( *psz1 < *psz2 )
        {
            return -1;
        }
		psz1++;
		psz2++;
		i++;
	}
    if ( str1_len < str2_len )
    {
        return -1;
    }
    else if ( str1_len > str2_len )
    {
        return 1;
    }
    else
    {
    	return 0;
    }
}


/*
 *	StrCompare( )
 *	Compare if two string is equal.
 *	Return Values:
 *		1		pStr1 greater than pStr2
 *		0		pStr1 equal to pStr2
 *      -1      pStr1 less than pStr2
 */
INT StrCompare( void *pStr1, void *pStr2 )
{
	return stricmp( (char *)pStr1, (char *)pStr2 );
}


/**	�����ıȽϺ���

	@param	void *pInt1 - ��1��Ҫ�Ƚϵ�����	
	@param	void *pInt2 - ��2��Ҫ�Ƚϵ�����	
	@return	INT -  1     p1 > pt2
                   0     p1 = p2
                  -1     p1 < p2
*/
INT IntCompare(void *p1, void *p2)
{
    if ( (INT)p1 > (INT)p2 )
    {
        return 1;
    }
    else if ( (INT)p1 < (INT)p2 )
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/**	�Ƚ�����ָ���С�ĺ���

    @param	void *p1 - Ҫ�Ƚϵ�ָ��1	
    @param	void *p2 - Ҫ�Ƚϵ�ָ��2
    @return	INT - 0��ʾ��ȣ�1��ʾp1>p2, -1��ʾp1<p2	
*/
INT PointerCompare(void *p1, void *p2)
{
    if ( p1 == p2 )
    {
        return 0;
    }
    else if ( (INT)p1 < (INT)p2 )
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

