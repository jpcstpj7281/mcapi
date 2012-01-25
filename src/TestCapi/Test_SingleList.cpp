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
#include "TestCapi.h"
#include <stdio.h>
#include "CapiGlobal.h"
#include "TestApi.h"
#include "SingleList.h"

void DRV_SingleList_Create(UINT i);
void DRV_SingleList_InsertHead(UINT i);
void DRV_SingleList_InsertTail(UINT i);
void DRV_SingleList_PopHead(UINT i);
void DRV_SingleList_PopTail(UINT i);
void DRV_SingleList_Find(UINT i);
void DRV_SingleList_InsertSort(UINT i);
void DRV_SingleList_MergeSort(UINT i);
void DRV_SingleList_RadixSort(UINT i);

void TestCase_SingleList_Pop(void);
void TestCase_SingleList_Delete(void);

void TestCase_SingleList_PopHead_EnumNext(void);
void TestCase_SingleList_PopTail_EnumNext(void);
void TestCase_SingleList_Pop_EnumNext(void);
void TestCase_SingleList_Delete_EnumNext(void);


void Test_SingleList()
{
    int     i;
    for ( i = 1; i < 50; i++ )
    {
        DRV_SingleList_Create( i);
        DRV_SingleList_InsertHead( i);
        DRV_SingleList_InsertTail( i);
        DRV_SingleList_PopHead( i);
        DRV_SingleList_PopTail( i);
        DRV_SingleList_Find( i);
        DRV_SingleList_InsertSort( i);
        DRV_SingleList_MergeSort( i);
//        DRV_SingleList_RadixSort( i);
    }  

    TestCase_Add(TestCase_SingleList_Pop);
    TestCase_Add(TestCase_SingleList_Delete);

    TestCase_Add(TestCase_SingleList_PopHead_EnumNext);
    TestCase_Add(TestCase_SingleList_PopTail_EnumNext);
    TestCase_Add(TestCase_SingleList_Pop_EnumNext);
    TestCase_Add(TestCase_SingleList_Delete_EnumNext);

}

REGISTER_TESTFUNC(Test_SingleList)

void DRV_SingleList_Create(UINT i)
{
    SINGLELIST *pList;
    switch( i )
    {
    case 1:
        pList = SingleList_Create();
        if ( pList->pHead != NULL 
            || pList->pTail != NULL 
            || pList->uCount != 0 
            || pList->pCur != NULL )
        {
            printf( "SingleList_Create() ²âÊÔÓÃÀý1 Ê§°Ü!\n" );
        }
        SingleList_Destroy(pList, free);
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    default:
        break;
    }
}

void DRV_SingleList_InsertHead(UINT i)
{
    SINGLELIST *pList;
    pList = SingleList_Create();
    switch( i )
    {
    case 1: /* Á´±íÎª¿ÕÊ±µÄ²åÈë */
        SingleList_InsertHead(pList, strdup("1234"));
        if ( strcmp((char *)(pList->pHead->pData), "1234") != 0 
            || pList->pTail != pList->pHead 
            || pList->pHead->pNext != NULL
            || pList->uCount != 1 
            || pList->pCur != NULL )
        {
            printf( "SingleList_InsertHead() ²âÊÔÓÃÀý1 Ê§°Ü!\n" );
        }
        break;
    case 2: /* ÒÑÓÐ1¸ö½ÚµãµÄ²åÈë */
        SingleList_InsertHead(pList, strdup("1234"));
        SingleList_InsertHead(pList, strdup("5678"));
        if ( strcmp((char *)(pList->pHead->pData), "5678") != 0 
            || strcmp((char *)(pList->pTail->pData), "1234") != 0 
            || pList->pTail != pList->pHead->pNext 
            || pList->pTail->pNext != NULL
            || pList->uCount != 2 
            || pList->pCur != NULL )
        {
            printf( "SingleList_InsertHead() ²âÊÔÓÃÀý2 Ê§°Ü!\n" );
        }
        break;
    case 3: /* ÒÑÓÐÁ½¸ö½ÚµãµÄ²åÈë */
        SingleList_InsertHead(pList, strdup("1234"));
        SingleList_InsertHead(pList, strdup("5678"));
        SingleList_InsertHead(pList, strdup("910"));
        if ( strcmp((char *)(pList->pHead->pData), "910") != 0 
            || strcmp((char *)(pList->pHead->pNext->pData), "5678") != 0 
            || strcmp((char *)(pList->pTail->pData), "1234") != 0 
            || pList->pTail != pList->pHead->pNext->pNext 
            || pList->pTail->pNext != NULL
            || pList->uCount != 3 
            || pList->pCur != NULL )
        {
            printf( "SingleList_InsertHead() ²âÊÔÓÃÀý3 Ê§°Ü!\n" );
        }
        break;
    case 4:
        break;
    default:
        break;
    }
    SingleList_Destroy(pList, free);
}

void DRV_SingleList_InsertTail(UINT i)
{
    SINGLELIST *pList;
    pList = SingleList_Create();
    switch( i )
    {
    case 1: /* Á´±íÎª¿ÕÊ±µÄ²åÈë */
        SingleList_InsertTail(pList, strdup("1234"));
        if ( strcmp((char *)(pList->pHead->pData), "1234") != 0 
            || pList->pTail != pList->pHead 
            || pList->pHead->pNext != NULL
            || pList->uCount != 1 
            || pList->pCur != NULL )
        {
            printf( "SingleList_InsertHead() ²âÊÔÓÃÀý1 Ê§°Ü!\n" );
        }
        break;
    case 2: /* ÒÑÓÐ1¸ö½ÚµãµÄ²åÈë */
        SingleList_InsertTail(pList, strdup("1234"));
        SingleList_InsertTail(pList, strdup("5678"));
        if ( strcmp((char *)(pList->pHead->pData), "1234") != 0 
            || strcmp((char *)(pList->pTail->pData), "5678") != 0 
            || pList->pTail != pList->pHead->pNext 
            || pList->pTail->pNext != NULL
            || pList->uCount != 2 
            || pList->pCur != NULL )
        {
            printf( "SingleList_InsertHead() ²âÊÔÓÃÀý2 Ê§°Ü!\n" );
        }
        break;
    case 3: /* ÒÑÓÐÁ½¸ö½ÚµãµÄ²åÈë */
        SingleList_InsertTail(pList, strdup("1234"));
        SingleList_InsertTail(pList, strdup("5678"));
        SingleList_InsertTail(pList, strdup("910"));
        if ( strcmp((char *)(pList->pHead->pData), "1234") != 0 
            || strcmp((char *)(pList->pHead->pNext->pData), "5678") != 0 
            || strcmp((char *)(pList->pTail->pData), "910") != 0 
            || pList->pTail != pList->pHead->pNext->pNext 
            || pList->pTail->pNext != NULL
            || pList->uCount != 3 
            || pList->pCur != NULL )
        {
            printf( "SingleList_InsertHead() ²âÊÔÓÃÀý3 Ê§°Ü!\n" );
        }
        break;
    case 4:
        break;
    default:
        break;
    }
    SingleList_Destroy(pList, free);
}

void DRV_SingleList_PopHead(UINT i)
{
    SINGLELIST *pList;
    void       *pData;
    pList = SingleList_Create();
    switch( i )
    {
    case 1:
        SingleList_InsertTail(pList, strdup("1234"));

        pData = SingleList_PopHead(pList);
        if ( strcmp((char *)pData, "1234") != 0 
            || pList->pTail != NULL 
            || pList->pHead != NULL
            || pList->uCount != 0 
            || pList->pCur != NULL )
        {
            printf( "SingleList_PopHead() ²âÊÔÓÃÀý1 Ê§°Ü!\n" );
        }
        free(pData);
        break;
    case 2:
        pData = SingleList_PopHead(pList);
        if ( pData != NULL
            || pList->pTail != NULL 
            || pList->pHead != NULL
            || pList->uCount != 0 
            || pList->pCur != NULL )
        {
            printf( "SingleList_PopHead() ²âÊÔÓÃÀý2 Ê§°Ü!\n" );
        }
        break;
    case 3:
        SingleList_InsertTail(pList, strdup("1234"));
        SingleList_InsertTail(pList, strdup("5678"));
        
        pData = SingleList_PopHead(pList);
        if ( strcmp((char *)pData, "1234") != 0 
            || strcmp((char *)(pList->pHead->pData), "5678") != 0 
            || pList->pTail != pList->pHead 
            || pList->pHead->pNext != NULL
            || pList->uCount != 1 
            || pList->pCur != NULL )
        {
            printf( "SingleList_PopHead() ²âÊÔÓÃÀý3 Ê§°Ü!\n" );
        }
        free(pData);
        break; 
    case 4:
        SingleList_InsertTail(pList, strdup("1234"));
        SingleList_InsertTail(pList, strdup("5678"));
        SingleList_InsertTail(pList, strdup("910"));
        
        pData = SingleList_PopHead(pList);
        if ( strcmp((char *)pData, "1234") != 0 
            || strcmp((char *)(pList->pHead->pData), "5678") != 0 
            || strcmp((char *)(pList->pTail->pData), "910") != 0 
            || pList->pTail != pList->pHead->pNext 
            || pList->pTail->pNext != NULL 
            || pList->uCount != 2
            || pList->pCur != NULL )
        {
            printf( "SingleList_PopHead() ²âÊÔÓÃÀý4 Ê§°Ü!\n" );
        }
        free(pData);
        break;
    case 5:
        SingleList_InsertTail(pList, strdup("1234"));
        SingleList_InsertTail(pList, strdup("5678"));
        SingleList_InsertTail(pList, strdup("910"));
        
        pData = SingleList_PopHead(pList);
        free(pData);
        pData = SingleList_PopHead(pList);
        if ( strcmp((char *)pData, "5678") != 0 
            || strcmp((char *)(pList->pHead->pData), "910") != 0 
            || strcmp((char *)(pList->pTail->pData), "910") != 0 
            || pList->pTail != pList->pHead 
            || pList->pTail->pNext != NULL 
            || pList->uCount != 1
            || pList->pCur != NULL )
        {
            printf( "SingleList_PopHead() ²âÊÔÓÃÀý5 Ê§°Ü!\n" );
        }
        free(pData);
        break;
    default:
        break;
    }
    SingleList_Destroy(pList, free);
}

void DRV_SingleList_PopTail(UINT i)
{
    SINGLELIST *pList;
    void       *pData;
    pList = SingleList_Create();
    switch( i )
    {
    case 1:
        SingleList_InsertTail(pList, strdup("1234"));
        
        pData = SingleList_PopTail(pList);
        if ( strcmp((char *)pData, "1234") != 0 
            || pList->pTail != NULL 
            || pList->pHead != NULL
            || pList->uCount != 0 
            || pList->pCur != NULL )
        {
            printf( "SingleList_PopTail() ²âÊÔÓÃÀý1 Ê§°Ü!\n" );
        }
        free(pData);
        break;
    case 2:
        pData = SingleList_PopTail(pList);
        if ( pData != NULL
            || pList->pTail != NULL 
            || pList->pHead != NULL
            || pList->uCount != 0 
            || pList->pCur != NULL )
        {
            printf( "SingleList_PopTail() ²âÊÔÓÃÀý2 Ê§°Ü!\n" );
        }
        break;
    case 3:
        SingleList_InsertTail(pList, strdup("1234"));
        SingleList_InsertTail(pList, strdup("5678"));
        
        pData = SingleList_PopTail(pList);
        if ( strcmp((char *)pData, "5678") != 0 
            || strcmp((char *)(pList->pHead->pData), "1234") != 0 
            || pList->pTail != pList->pHead 
            || pList->pHead->pNext != NULL
            || pList->uCount != 1 
            || pList->pCur != NULL )
        {
            printf( "SingleList_PopTail() ²âÊÔÓÃÀý3 Ê§°Ü!\n" );
        }
        free(pData);
        break; 
    case 4:
        SingleList_InsertTail(pList, strdup("1234"));
        SingleList_InsertTail(pList, strdup("5678"));
        SingleList_InsertTail(pList, strdup("910"));
        
        pData = SingleList_PopTail(pList);
        if ( strcmp((char *)pData, "910") != 0 
            || strcmp((char *)(pList->pHead->pData), "1234") != 0 
            || strcmp((char *)(pList->pTail->pData), "5678") != 0 
            || pList->pTail != pList->pHead->pNext 
            || pList->pTail->pNext != NULL 
            || pList->uCount != 2
            || pList->pCur != NULL )
        {
            printf( "SingleList_PopTail() ²âÊÔÓÃÀý4 Ê§°Ü!\n" );
        }
        free(pData);
        break;
    case 5:
        SingleList_InsertTail(pList, strdup("1234"));
        SingleList_InsertTail(pList, strdup("5678"));
        SingleList_InsertTail(pList, strdup("910"));
        
        pData = SingleList_PopTail(pList);
        free(pData);
        pData = SingleList_PopTail(pList);
        if ( strcmp((char *)pData, "5678") != 0 
            || strcmp((char *)(pList->pHead->pData), "1234") != 0 
            || strcmp((char *)(pList->pTail->pData), "1234") != 0 
            || pList->pTail != pList->pHead 
            || pList->pTail->pNext != NULL 
            || pList->uCount != 1
            || pList->pCur != NULL )
        {
            printf( "SingleList_PopTail() ²âÊÔÓÃÀý5 Ê§°Ü!\n" );
        }
        free(pData);
        break;
    default:
        break;
    }
    SingleList_Destroy(pList, free);
}

void DRV_SingleList_Find(UINT i)
{
    SINGLELIST *pList = NULL;
    switch( i )
    {
    case 1:
        pList = SingleList_Create();
        if ( pList->pHead != NULL 
            || pList->pTail != NULL 
            || pList->uCount != 0 
            || pList->pCur != NULL )
        {
            printf( "SingleList_Create() ²âÊÔÓÃÀý1 Ê§°Ü!\n" );
        }
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    default:
        break;
    }
    SingleList_Destroy(pList, free);
}


SINGLELIST *BuildSortCase1()
{
    SINGLELIST *pList = SingleList_Create();

    if ( pList != NULL )
    {
        SingleList_InsertTail( pList, strdup("19"));
        SingleList_InsertTail( pList, strdup("20"));
        SingleList_InsertTail( pList, strdup("22"));
        SingleList_InsertTail( pList, strdup("23"));
        SingleList_InsertTail( pList, strdup("21"));
        SingleList_InsertTail( pList, strdup("18"));
        SingleList_InsertTail( pList, strdup("11"));
        SingleList_InsertTail( pList, strdup("10"));
        SingleList_InsertTail( pList, strdup("17"));
        SingleList_InsertTail( pList, strdup("15"));
        SingleList_InsertTail( pList, strdup("12"));
        SingleList_InsertTail( pList, strdup("13"));
        SingleList_InsertTail( pList, strdup("16"));
        SingleList_InsertTail( pList, strdup("14"));
        pList->pCur = pList->pHead->pNext->pNext->pNext->pNext->pNext->pNext->pNext->pNext->pNext;
    }
    return pList;
}

SINGLELIST *BuildSortCase2()
{
    SINGLELIST *pList = SingleList_Create();
    
    if ( pList != NULL )
    {
        SingleList_InsertTail( pList, strdup("19"));
        SingleList_InsertTail( pList, strdup("18"));
        SingleList_InsertTail( pList, strdup("17"));
        SingleList_InsertTail( pList, strdup("16"));
        SingleList_InsertTail( pList, strdup("15"));
        SingleList_InsertTail( pList, strdup("14"));
        SingleList_InsertTail( pList, strdup("13"));
        SingleList_InsertTail( pList, strdup("12"));
        SingleList_InsertTail( pList, strdup("11"));
        SingleList_InsertTail( pList, strdup("10"));
        pList->pCur = pList->pHead->pNext->pNext->pNext->pNext;
    }
    return pList;
}


SINGLELIST *BuildSortCase3()
{
    SINGLELIST *pList = SingleList_Create();
    
    if ( pList != NULL )
    {
        SingleList_InsertTail( pList, strdup("10"));
        SingleList_InsertTail( pList, strdup("11"));
        SingleList_InsertTail( pList, strdup("12"));
        SingleList_InsertTail( pList, strdup("13"));
        SingleList_InsertTail( pList, strdup("14"));
        SingleList_InsertTail( pList, strdup("15"));
        SingleList_InsertTail( pList, strdup("16"));
        SingleList_InsertTail( pList, strdup("17"));
        SingleList_InsertTail( pList, strdup("18"));
        SingleList_InsertTail( pList, strdup("19"));
        pList->pCur = pList->pHead->pNext->pNext->pNext->pNext->pNext;
    }
    return pList;
}

INT CheckSortCase(SINGLELIST *pList, INT nMax)
{
    SINGLENODE  *pNode;
    char        psz[16];
    char        *pszSrc;

    INT         n = 10;

    itoa(n, psz, 10);

    pNode = pList->pHead;

    pszSrc = (char *)(pNode->pData);

    while ( strcmp(pszSrc, psz) == 0 )
    {
        n += 1;
        itoa(n, psz, 10);
        pNode = pNode->pNext;
        if ( pNode != NULL )
        {
            pszSrc = (char *)(pNode->pData);
        }
        else
        {
            break;
        }
    }

    if ( n < nMax )
    {
        return 0;
    }

    itoa(nMax -1 , psz, 10);
    if ( strcmp((char *)(pList->pTail->pData), psz) != 0 )
    {
        return 0;
    }

    if ( pList->pCur != NULL && strcmp((char *)(pList->pCur->pData), "15" ) != 0 )
    {
        return 0;
    }

    if ( pList->uCount != (UINT)(nMax - 10) )
    {
        return 0;
    }

    return 1;

}


void DRV_SingleList_InsertSort(UINT i)
{
    SINGLELIST *pList = NULL;
    switch( i )
    {
    case 1:
        pList = BuildSortCase1();
        SingleList_InsertSort(pList, StrCompare);
        if ( CheckSortCase(pList, 24) == 0 )
        {
            printf( "InsertSort ²âÊÔÓÃÀý1Ê§°Ü\n" );
        }
        break;
    case 2:
        pList = BuildSortCase2();
        SingleList_InsertSort(pList, StrCompare);
        if ( CheckSortCase(pList, 20) == 0 )
        {
            printf( "InsertSort ²âÊÔÓÃÀý2Ê§°Ü\n" );
        }
        break;
    case 3:
        pList = BuildSortCase3();
        SingleList_InsertSort(pList, StrCompare);
        if ( CheckSortCase(pList, 20) == 0 )
        {
            printf( "InsertSort ²âÊÔÓÃÀý3Ê§°Ü\n" );
        }
        break;
    case 4:
        pList = SingleList_Create();
        SingleList_InsertHead(pList, strdup("10"));
        SingleList_InsertSort(pList, StrCompare);
        if ( CheckSortCase(pList, 11) == 0 )
        {
            printf( "InsertSort ²âÊÔÓÃÀý4Ê§°Ü\n" );
        }
        break;
    case 5:
        pList = SingleList_Create();
        SingleList_InsertHead(pList, strdup("10"));
        SingleList_InsertHead(pList, strdup("11"));
        SingleList_InsertSort(pList, StrCompare);
        if ( CheckSortCase(pList, 12) == 0 )
        {
            printf( "InsertSort ²âÊÔÓÃÀý5Ê§°Ü\n" );
        }
        break;
    case 6:
        pList = SingleList_Create();
        SingleList_InsertSort(pList, StrCompare);
        if ( pList->pHead != NULL 
            || pList->pTail != NULL 
            || pList->pCur != NULL 
            || pList->uCount != 0 )
        {
            printf( "InsertSort ²âÊÔÓÃÀý6Ê§°Ü\n" );
        }
        break;
    default:
        break;
    }
    if ( pList != NULL )
    {
        SingleList_Destroy(pList, free);
    }
}

void DRV_SingleList_MergeSort(UINT i)
{
    SINGLELIST *pList = NULL;
    switch( i )
    {
    case 1:
        pList = BuildSortCase1();
        SingleList_MergeSort(pList, StrCompare, 0);
        if ( CheckSortCase(pList, 24) == 0 )
        {
            printf( "MergeSort ²âÊÔÓÃÀý1Ê§°Ü\n" );
        }
        break;
    case 2:
        pList = BuildSortCase2();
        SingleList_MergeSort(pList, StrCompare, 0);
        if ( CheckSortCase(pList, 20) == 0 )
        {
            printf( "MergeSort ²âÊÔÓÃÀý2Ê§°Ü\n" );
        }
        break;
    case 3:
        pList = BuildSortCase3();
        SingleList_MergeSort(pList, StrCompare, 2);
        if ( CheckSortCase(pList, 20) == 0 )
        {
            printf( "MergeSort ²âÊÔÓÃÀý3Ê§°Ü\n" );
        }
        break;
    case 4:
        pList = SingleList_Create();
        SingleList_InsertHead(pList, strdup("10"));
        SingleList_MergeSort(pList, StrCompare, 0);
        if ( CheckSortCase(pList, 11) == 0 )
        {
            printf( "MergeSort ²âÊÔÓÃÀý4Ê§°Ü\n" );
        }
        break;
    case 5:
        pList = SingleList_Create();
        SingleList_InsertHead(pList, strdup("10"));
        SingleList_InsertHead(pList, strdup("11"));
        SingleList_MergeSort(pList, StrCompare, 0);
        if ( CheckSortCase(pList, 12) == 0 )
        {
            printf( "MergeSort ²âÊÔÓÃÀý5Ê§°Ü\n" );
        }
        break;
    case 6:
        pList = SingleList_Create();
        SingleList_MergeSort(pList, StrCompare, 0);
        if ( pList->pHead != NULL 
            || pList->pTail != NULL 
            || pList->pCur != NULL 
            || pList->uCount != 0 )
        {
            printf( "MergeSort ²âÊÔÓÃÀý6Ê§°Ü\n" );
        }
        break;
    default:
        break;
    }
    if ( pList != NULL )
    {
        SingleList_Destroy(pList, free);
    }
}

#if 0
void DRV_SingleList_RadixSort(UINT i)
{
    SINGLELIST *pList = NULL;
    switch( i )
    {
    case 1:
        pList = BuildSortCase1();
        SingleList_RadixSort(pList, 256, 5, GetStrKey);
        if ( CheckSortCase(pList, 24) == 0 )
        {
            printf( "RadixSort ²âÊÔÓÃÀý1Ê§°Ü\n" );
        }
        break;
    case 2:
        pList = BuildSortCase2();
        SingleList_RadixSort(pList, 256, 5, GetStrKey);
        if ( CheckSortCase(pList, 20) == 0 )
        {
            printf( "RadixSort ²âÊÔÓÃÀý2Ê§°Ü\n" );
        }
        break;
    case 3:
        pList = BuildSortCase3();
        SingleList_RadixSort(pList, 256, 5, GetStrKey);
        if ( CheckSortCase(pList, 20) == 0 )
        {
            printf( "RadixSort ²âÊÔÓÃÀý3Ê§°Ü\n" );
        }
        break;
    case 4:
        pList = SingleList_Create();
        SingleList_InsertHead(pList, strdup("10"));
        SingleList_RadixSort(pList, 256, 5, GetStrKey);
        if ( CheckSortCase(pList, 11) == 0 )
        {
            printf( "RadixSort ²âÊÔÓÃÀý4Ê§°Ü\n" );
        }
        break;
    case 5:
        pList = SingleList_Create();
        SingleList_InsertHead(pList, strdup("10"));
        SingleList_InsertHead(pList, strdup("11"));
        SingleList_RadixSort(pList, 256, 5, GetStrKey);
        if ( CheckSortCase(pList, 12) == 0 )
        {
            printf( "RadixSort ²âÊÔÓÃÀý5Ê§°Ü\n" );
        }
        break;
    case 6:
        pList = SingleList_Create();
        SingleList_RadixSort(pList, 256, 5, GetStrKey);
        if ( pList->pHead != NULL 
            || pList->pTail != NULL 
            || pList->pCur != NULL 
            || pList->uCount != 0 )
        {
            printf( "RadixSort ²âÊÔÓÃÀý6Ê§°Ü\n" );
        }
        break;
    default:
        break;
    }
    if ( pList != NULL )
    {
        SingleList_Destroy(pList, free);
    }
}
#endif


void TestCase_SingleList_Pop(void)
{
    SINGLELIST *pList;
    void *pData;

    pList = SingleList_Create();

    pData = SingleList_Pop(pList, (void *)100, IntCompare);

    assertTrue(pData == NULL);


    SingleList_InsertHead(pList, (void *)100);

    pData = SingleList_Pop(pList, (void *)100, IntCompare);

    assertTrue(pData == (void *)100 
        && pList->pHead == NULL
        && pList->pTail == NULL
        && pList->uCount == 0 );

    pData = SingleList_Pop(pList, (void *)100, IntCompare);

    assertTrue(pData == NULL);


    SingleList_InsertHead(pList, (void *)100);
    SingleList_InsertTail(pList, (void *)101);

    pData = SingleList_Pop(pList, (void *)101, IntCompare);

    assertTrue(pData == (void *)101 
        && pList->pHead->pData == (void *)100
        && pList->pHead->pNext == NULL 
        && pList->pTail->pData == (void *)100
        && pList->uCount == 1 );

    pData = SingleList_Pop(pList, (void *)100, IntCompare);

    assertTrue(pData == (void *)100 
        && pList->pHead == NULL
        && pList->pTail == NULL
        && pList->uCount == 0 );
    pData = SingleList_Pop(pList, (void *)100, IntCompare);

    assertTrue(pData == NULL 
        && pList->uCount == 0
        && pList->pHead == NULL
        && pList->pTail == NULL );


    SingleList_InsertHead(pList, (void *)100);
    SingleList_InsertTail(pList, (void *)101);
    SingleList_InsertTail(pList, (void *)102);

    pData = SingleList_Pop(pList, (void *)101, IntCompare);

    assertTrue(pData == (void *)101 
        && pList->pHead->pData == (void *)100
        && pList->pHead->pNext == pList->pTail 
        && pList->pTail->pData == (void *)102
        && pList->pTail->pNext == NULL
        && pList->uCount == 2 );

    return;
}


void TestCase_SingleList_Delete(void)
{
    SINGLELIST *pList;
    void *pData;
    INT nRet;

    pList = SingleList_Create();

    pData = SingleList_Pop(pList, (void *)100, IntCompare);

    assertTrue(pData == NULL);


    SingleList_InsertHead(pList, (void *)100);

    nRet = SingleList_Delete(pList, (void *)100, IntCompare, NULL);

    assertTrue( nRet == CAPI_SUCCESS
        && pList->pHead == NULL
        && pList->pTail == NULL
        && pList->uCount == 0 );

    nRet = SingleList_Delete(pList, (void *)100, IntCompare, NULL);

    assertTrue(nRet == CAPI_FAILED);


    SingleList_InsertHead(pList, (void *)100);
    SingleList_InsertTail(pList, (void *)101);

    nRet = SingleList_Delete(pList, (void *)101, IntCompare, NULL);

    assertTrue( nRet == CAPI_SUCCESS && pList->pHead->pData == (void *)100
        && pList->pHead->pNext == NULL 
        && pList->pTail->pData == (void *)100
        && pList->uCount == 1 );

    nRet = SingleList_Delete(pList, (void *)100, IntCompare, NULL);

    assertTrue( nRet == CAPI_SUCCESS && pList->pHead == NULL
        && pList->pTail == NULL
        && pList->uCount == 0 );
    SingleList_Delete(pList, (void *)100, IntCompare, NULL);

    assertTrue(pList->uCount == 0
        && pList->pHead == NULL
        && pList->pTail == NULL );


    SingleList_InsertHead(pList, (void *)100);
    SingleList_InsertTail(pList, (void *)101);
    SingleList_InsertTail(pList, (void *)102);

    nRet = SingleList_Delete(pList, (void *)101, IntCompare, NULL);

    assertTrue( nRet == CAPI_SUCCESS 
        && pList->pHead->pData == (void *)100
        && pList->pHead->pNext == pList->pTail 
        && pList->pTail->pData == (void *)102
        && pList->pTail->pNext == NULL
        && pList->uCount == 2 );

    return;
}



void TestCase_SingleList_PopHead_EnumNext(void)
{
    SINGLELIST *pList;
    void *pData;

    pList = SingleList_Create();

    SingleList_EnumBegin(pList);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);


    SingleList_InsertHead(pList, (void *)100);

    SingleList_EnumBegin(pList);

    SingleList_PopHead(pList);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);


    SingleList_InsertHead(pList, (void *)100);

    SingleList_EnumBegin(pList);
    pData = SingleList_EnumNext(pList);
    assertTrue(pData == (void *)100);

    SingleList_PopHead(pList);
    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);


    SingleList_InsertHead(pList, (void *)100);
    SingleList_InsertHead(pList, (void *)101);

    SingleList_EnumBegin(pList);

    SingleList_PopHead(pList);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == (void *)100);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);



    return;

}


void TestCase_SingleList_PopTail_EnumNext(void)
{
    SINGLELIST *pList;
    void *pData;

    pList = SingleList_Create();

    SingleList_EnumBegin(pList);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);


    SingleList_InsertTail(pList, (void *)100);

    SingleList_EnumBegin(pList);

    SingleList_PopTail(pList);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);


    SingleList_InsertTail(pList, (void *)100);
    SingleList_InsertTail(pList, (void *)101);

    SingleList_EnumBegin(pList);

    SingleList_PopTail(pList);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == (void *)100);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);


    SingleList_InsertTail(pList, (void *)101);

    SingleList_EnumBegin(pList);


    pData = SingleList_EnumNext(pList);

    assertTrue(pData == (void *)100);
    SingleList_PopTail(pList);

    pData = SingleList_EnumNext(pList);
    assertTrue(pData == NULL);


    SingleList_InsertTail(pList, (void *)101);
    SingleList_InsertTail(pList, (void *)102);

    SingleList_EnumBegin(pList);
    pData = SingleList_EnumNext(pList);
    assertTrue(pData == (void *)100);
    pData = SingleList_EnumNext(pList);
    assertTrue(pData == (void *)101);

    SingleList_PopTail(pList);

    pData = SingleList_EnumNext(pList);
    assertTrue(pData == NULL);


    return;

}


void TestCase_SingleList_Pop_EnumNext(void)
{
    SINGLELIST *pList;
    void *pData;

    pList = SingleList_Create();

    SingleList_EnumBegin(pList);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);


    SingleList_InsertHead(pList, (void *)100);

    SingleList_EnumBegin(pList);

    SingleList_Pop(pList, (void *)100, IntCompare);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);


    SingleList_InsertHead(pList, (void *)100);
    SingleList_InsertHead(pList, (void *)101);

    SingleList_EnumBegin(pList);

    SingleList_Pop(pList, (void *)101, IntCompare);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == (void *)100);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);


    SingleList_InsertHead(pList, (void *)101);
    SingleList_InsertHead(pList, (void *)102);

    SingleList_EnumBegin(pList);
    pData = SingleList_EnumNext(pList);
    assertTrue(pData == (void *)102);

    SingleList_Pop(pList, (void *)101, IntCompare);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == (void *)100);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);


    return;
}


void TestCase_SingleList_Delete_EnumNext(void)
{
    SINGLELIST *pList;
    void *pData;

    pList = SingleList_Create();

    SingleList_EnumBegin(pList);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);


    SingleList_InsertHead(pList, (void *)100);

    SingleList_EnumBegin(pList);

    SingleList_Delete(pList, (void *)100, IntCompare, NULL);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);


    SingleList_InsertHead(pList, (void *)100);
    SingleList_InsertHead(pList, (void *)101);

    SingleList_EnumBegin(pList);

    SingleList_Delete(pList, (void *)101, IntCompare, NULL);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == (void *)100);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);


    SingleList_InsertHead(pList, (void *)101);
    SingleList_InsertHead(pList, (void *)102);

    SingleList_EnumBegin(pList);
    pData = SingleList_EnumNext(pList);

    SingleList_Delete(pList, (void *)101, IntCompare, NULL);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == (void *)100);

    pData = SingleList_EnumNext(pList);

    assertTrue(pData == NULL);


    return;

}

