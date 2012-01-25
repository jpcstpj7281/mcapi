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
#include "BinTree.h"
#include "AVLTree.h"
#include "HashAVLTree.h"


void DRV_HashAVLTree_Create(UINT i);
void DRV_HashAVLTree_Insert(UINT i);
void DRV_HashAVLTree_Delete(UINT i);
void DRV_HashAVLTree_Find(UINT i);
void DRV_HashAVLTree_EnumNext(UINT i);

void TestCase_HashAVLTree_EnumNext(void);
void TestCase_HashAVLTree_EnumNextAndDelete(void);


typedef HASHAVLTREE HashAVLTree;


void Test_HashAVLTree()
{
    int     i;
    for ( i = 1; i < 50; i++ )
    {
        DRV_HashAVLTree_Create(i);
        DRV_HashAVLTree_Insert(i);
        DRV_HashAVLTree_Delete(i);
        DRV_HashAVLTree_Find(i);
        DRV_HashAVLTree_EnumNext(i);
        
    }  

    TestCase_Add(TestCase_HashAVLTree_EnumNext);
    TestCase_Add(TestCase_HashAVLTree_EnumNextAndDelete);
}


REGISTER_TESTFUNC(Test_HashAVLTree)



void DRV_HashAVLTree_Create(UINT i)
{
    HashAVLTree *pTable = NULL;
    switch( i )
    {
    case 1:
        pTable = HashAVLTree_Create(0);
        if ( pTable != NULL )
        {           
            printf( "HashAVLTree_Create(0) ²âÊÔÓÃÀý1Ê§°Ü!\n" );
        }
        break;
    case 2:
        pTable = HashAVLTree_Create(1);
        if ( pTable == NULL
			|| pTable->ppBucket == NULL 
			|| pTable->pCurEntry != NULL
			|| pTable->uBucketCount != 1
			|| pTable->uCurBucketNo != 0
			|| pTable->uNodeCount != 0  )
        {           
            printf( "HashAVLTree_Create(1) ²âÊÔÓÃÀý2Ê§°Ü!\n" );
        }
        break;
    case 3:
        pTable = HashAVLTree_Create(2);
        if ( pTable == NULL
			|| pTable->ppBucket == NULL 
			|| pTable->pCurEntry != NULL
			|| pTable->uBucketCount != 2
			|| pTable->uCurBucketNo != 0
			|| pTable->uNodeCount != 0  )
        {           
            printf( "HashAVLTree_Create(2) ²âÊÔÓÃÀý3Ê§°Ü!\n" );
        }
        break;
    case 4:
        pTable = HashAVLTree_Create(1024);
        if ( pTable == NULL
			|| pTable->ppBucket == NULL 
			|| pTable->pCurEntry != NULL
			|| pTable->uBucketCount != 1024
			|| pTable->uCurBucketNo != 0
			|| pTable->uNodeCount != 0  )
        {           
            printf( "HashAVLTree_Create(1024) ²âÊÔÓÃÀý4Ê§°Ü!\n" );
        }
        break;
    default:
        break;
    }
    if ( pTable != NULL && pTable->uBucketCount != 0  )
    {
        HashAVLTree_Destroy(pTable, NULL);
    }
}

void DRV_HashAVLTree_Insert(UINT i)
{
    INT         nRet;
    void        *pData;
    UINT         uIndex;
    HashAVLTree *pTable = NULL;
    pTable = HashAVLTree_Create(128);
    uIndex = HashString("100", 128);
	
    switch( i )
    {
    case 1:
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        if ( nRet != CAPI_SUCCESS
            || strcmp((char *)pTable->ppBucket[uIndex]->pData, "100") != 0 
            || pTable->ppBucket[uIndex]->pRight != NULL 
            || pTable->ppBucket[uIndex]->pLeft != NULL 
            || pTable->ppBucket[uIndex]->pParent != NULL 
            || pTable->ppBucket[uIndex]->nMagic != NULL 
			|| pTable->pCurEntry != NULL
			|| pTable->uCurBucketNo != 0
			|| pTable->uNodeCount != 1  
            || pTable->uBucketCount != 128 )
        {           
            printf( "HashAVLTree_Insert() ²âÊÔÓÃÀý1Ê§°Ü!\n" );
        }
        break;
    case 2:
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        pData = strdup("100");
        nRet = HashAVLTree_Insert(pTable, pData, HashString, StrCompare);
        if ( nRet == CAPI_FAILED )
        {
            free(pData);
        }
        if ( nRet != CAPI_FAILED
            || strcmp((char *)pTable->ppBucket[uIndex]->pData, "100") != 0 
            || pTable->ppBucket[uIndex]->pRight != NULL 
            || pTable->ppBucket[uIndex]->pLeft != NULL 
            || pTable->ppBucket[uIndex]->pParent != NULL 
            || pTable->ppBucket[uIndex]->nMagic != 0 
			|| pTable->pCurEntry != NULL
			|| pTable->uCurBucketNo != 0
			|| pTable->uNodeCount != 1  
            || pTable->uBucketCount != 128 )
        {           
            printf( "HashAVLTree_Insert() ²âÊÔÓÃÀý2Ê§°Ü!\n" );
        }
        break;
    case 3:
        nRet = HashAVLTree_Insert(pTable, strdup("252"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("728"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        if ( nRet != CAPI_SUCCESS
            || strcmp((char *)pTable->ppBucket[uIndex]->pData, "252") != 0 
            || strcmp((char *)pTable->ppBucket[uIndex]->pLeft->pData, "100") != 0 
            || strcmp((char *)pTable->ppBucket[uIndex]->pRight->pData, "728") != 0 
            || pTable->ppBucket[uIndex]->pRight->pLeft != NULL 
            || pTable->ppBucket[uIndex]->pRight->pRight != NULL 
            || pTable->ppBucket[uIndex]->pLeft->pLeft != NULL 
            || pTable->ppBucket[uIndex]->pLeft->pRight != NULL 
			|| strcmp((char *)pTable->ppBucket[uIndex]->pLeft->pParent->pData, "252") != 0
			|| strcmp((char *)pTable->ppBucket[uIndex]->pRight->pParent->pData, "252") != 0
            || pTable->ppBucket[uIndex]->nMagic != 0 
			|| pTable->pCurEntry != NULL
			|| pTable->uCurBucketNo != 0
			|| pTable->uNodeCount != 3  
            || pTable->uBucketCount != 128 )
        {           
            printf( "HashAVLTree_Insert() ²âÊÔÓÃÀý3Ê§°Ü!\n" );
        }

        break;
    default:
        break;
    }
    if ( pTable != NULL && pTable->uBucketCount != 0  )
    {
        HashAVLTree_Destroy(pTable, free);
    }
}


void DRV_HashAVLTree_Delete(UINT i)
{
    INT         nRet;
    UINT         uIndex;
    HashAVLTree *pTable = NULL;
    pTable = HashAVLTree_Create(128);
    uIndex = HashString("100", 128);
	
    switch( i )
    {
    case 1:
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        HashAVLTree_Delete(pTable, "100", HashString, StrCompare, free);

        if ( nRet != CAPI_SUCCESS
			|| pTable->ppBucket == NULL 
			|| pTable->ppBucket[uIndex] != NULL
			|| pTable->pCurEntry != NULL
			|| pTable->uBucketCount != 128
			|| pTable->uCurBucketNo != 0
			|| pTable->uNodeCount != 0  )
        {           
            printf( "HashAVLTree_Delete() ²âÊÔÓÃÀý1Ê§°Ü!\n" );
        }
        break;
    case 2:
        nRet = HashAVLTree_Insert(pTable, strdup("252"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("728"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        HashAVLTree_Delete(pTable, "728", HashString, StrCompare, free);
        if ( nRet != CAPI_SUCCESS
            || strcmp((char *)pTable->ppBucket[uIndex]->pData, "252") != 0 
            || strcmp((char *)pTable->ppBucket[uIndex]->pLeft->pData, "100") != 0 
            || pTable->ppBucket[uIndex]->pLeft->pRight != NULL 
            || pTable->ppBucket[uIndex]->pLeft->pLeft != NULL 
            || pTable->ppBucket[uIndex]->pRight != NULL 
			|| strcmp((char *)pTable->ppBucket[uIndex]->pLeft->pParent->pData, "252") != 0
            || pTable->ppBucket[uIndex]->nMagic != 1 
			|| pTable->pCurEntry != NULL
			|| pTable->uCurBucketNo != 0
			|| pTable->uNodeCount != 2  
            || pTable->uBucketCount != 128 )        
        {           
            printf( "HashAVLTree_Delete() ²âÊÔÓÃÀý2Ê§°Ü!\n" );
        }
        
        break;
    case 3:
        nRet = HashAVLTree_Insert(pTable, strdup("252"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("728"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        HashAVLTree_Delete(pTable, "100", HashString, StrCompare, free);
        if ( nRet != CAPI_SUCCESS
            || strcmp((char *)pTable->ppBucket[uIndex]->pData, "252") != 0 
            || strcmp((char *)pTable->ppBucket[uIndex]->pRight->pData, "728") != 0 
            || pTable->ppBucket[uIndex]->pRight->pLeft != NULL 
            || pTable->ppBucket[uIndex]->pRight->pRight != NULL 
            || pTable->ppBucket[uIndex]->pLeft != NULL 
			|| strcmp((char *)pTable->ppBucket[uIndex]->pRight->pParent->pData, "252") != 0
            || pTable->ppBucket[uIndex]->nMagic != -1 
			|| pTable->pCurEntry != NULL
			|| pTable->uCurBucketNo != 0
			|| pTable->uNodeCount != 2  
            || pTable->uBucketCount != 128 )        
		{           
            printf( "HashAVLTree_Delete() ²âÊÔÓÃÀý3Ê§°Ü!\n" );
        }
        
        break;
    case 4:
        nRet = HashAVLTree_Insert(pTable, strdup("252"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("728"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        HashAVLTree_Delete(pTable, "252", HashString, StrCompare, free);
        if ( nRet != CAPI_SUCCESS
            || strcmp((char *)pTable->ppBucket[uIndex]->pData, "100") != 0 
            || strcmp((char *)pTable->ppBucket[uIndex]->pRight->pData, "728") != 0 
            || pTable->ppBucket[uIndex]->pRight->pLeft != NULL 
            || pTable->ppBucket[uIndex]->pRight->pRight != NULL 
            || pTable->ppBucket[uIndex]->pLeft != NULL 
			|| strcmp((char *)pTable->ppBucket[uIndex]->pRight->pParent->pData, "100") != 0
            || pTable->ppBucket[uIndex]->nMagic != -1 
			|| pTable->pCurEntry != NULL
			|| pTable->uCurBucketNo != 0
			|| pTable->uNodeCount != 2  
            || pTable->uBucketCount != 128 )        
        {           
            printf( "HashAVLTree_Delete() ²âÊÔÓÃÀý4Ê§°Ü!\n" );
        }
        
        break;
    default:
        break;
    }
    if ( pTable != NULL && pTable->uBucketCount != 0  )
    {
        HashAVLTree_Destroy(pTable, free);
    }
}

void DRV_HashAVLTree_Find(UINT i)
{
    void        *pData;
    INT         nRet;
    HashAVLTree *pTable = NULL;
    pTable = HashAVLTree_Create(128);
    switch( i )
    {
    case 1:
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);

        pData = HashAVLTree_Find(pTable, "100", HashString, StrCompare);
        if ( strcmp((char *)pData, "100") != 0 )
        {           
            printf( "HashAVLTree_HashFind() ²âÊÔÓÃÀý1Ê§°Ü!\n" );
        }
        break;
    case 2:
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
//        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        pData = HashAVLTree_Find(pTable, "100", HashString, StrCompare);
        if ( strcmp((char *)pData, "100") != 0 )
        {           
            printf( "HashAVLTree_HashFind() ²âÊÔÓÃÀý1Ê§°Ü!\n" );
        }
        break;
    case 3:
        nRet = HashAVLTree_Insert(pTable, strdup("252"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("728"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        pData = HashAVLTree_Find(pTable, "252", HashString, StrCompare);
        if ( strcmp((char *)pData, "252") != 0 )
        {           
            printf( "HashAVLTree_HashFind() ²âÊÔÓÃÀý3Ê§°Ü!\n" );
        }
        break;
    case 4:
        nRet = HashAVLTree_Insert(pTable, strdup("252"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("728"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        pData = HashAVLTree_Find(pTable, "728", HashString, StrCompare);
        if ( strcmp((char *)pData, "728") != 0 )
        {           
            printf( "HashAVLTree_HashFind() ²âÊÔÓÃÀý4Ê§°Ü!\n" );
        }
        break;    
    case 5:
        nRet = HashAVLTree_Insert(pTable, strdup("252"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("728"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        pData = HashAVLTree_Find(pTable, "100", HashString, StrCompare);
        if ( strcmp((char *)pData, "100") != 0 )
        {           
            printf( "HashAVLTree_HashFind() ²âÊÔÓÃÀý5Ê§°Ü!\n" );
        }
        break;    
    case 6:
        nRet = HashAVLTree_Insert(pTable, strdup("252"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("728"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("45"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("300"), HashString, StrCompare);
        pData = HashAVLTree_Find(pTable, "45", HashString, StrCompare);
        if ( strcmp((char *)pData, "45") != 0 )
        {           
            printf( "HashAVLTree_HashFind() ²âÊÔÓÃÀý6Ê§°Ü!\n" );
        }
        break;    
    default:
        break;
    }
    if ( pTable != NULL && pTable->uBucketCount != 0  )
    {
        HashAVLTree_Destroy(pTable, free);
    }
}

void DRV_HashAVLTree_EnumNext(UINT i)
{
    void        *pData;
    INT         nRet;
    HashAVLTree *pTable = NULL;
    pTable = HashAVLTree_Create(128);
    switch( i )
    {
    case 1:
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        
        HashAVLTree_EnumBegin(pTable);
        pData = HashAVLTree_EnumNext(pTable);
        if ( strcmp((char *)pData, "100") != 0 )
        {           
            printf( "HashAVLTree_EnumNext() ²âÊÔÓÃÀý1Ê§°Ü!\n" );
        }
        pData = HashAVLTree_EnumNext(pTable);
        if ( pData != NULL )
        {           
            printf( "HashAVLTree_EnumNext() ²âÊÔÓÃÀý1Ê§°Ü!\n" );
        }

        break;

    case 2:
        nRet = HashAVLTree_Insert(pTable, strdup("252"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("728"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        HashAVLTree_EnumBegin(pTable);
        pData = HashAVLTree_EnumNext(pTable);
        if ( strcmp((char *)pData, "100") != 0 )
        {           
            printf( "HashAVLTree_EnumNext() ²âÊÔÓÃÀý2Ê§°Ü1!\n" );
        }
        pData = HashAVLTree_EnumNext(pTable);
        if ( strcmp((char *)pData, "252") != 0 )
        {           
            printf( "HashAVLTree_EnumNext() ²âÊÔÓÃÀý2Ê§°Ü2!\n" );
        }
        pData = HashAVLTree_EnumNext(pTable);
        if ( strcmp((char *)pData, "728") != 0 )
        {           
            printf( "HashAVLTree_EnumNext() ²âÊÔÓÃÀý2Ê§°Ü3!\n" );
        }
        pData = HashAVLTree_EnumNext(pTable);
        if ( pData != NULL )
        {           
            printf( "HashAVLTree_EnumNext() ²âÊÔÓÃÀý2Ê§°Ü4!\n" );
        }

        break;
    case 4:
        nRet = HashAVLTree_Insert(pTable, strdup("88"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("77"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("100"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("252"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("101"), HashString, StrCompare);
        nRet = HashAVLTree_Insert(pTable, strdup("728"), HashString, StrCompare);
        HashAVLTree_EnumBegin(pTable);
        pData = HashAVLTree_EnumNext(pTable);
        if ( strcmp((char *)pData, "77") != 0 )
        {           
            printf( "HashAVLTree_EnumNext() ²âÊÔÓÃÀý4Ê§°Ü1!\n" );
        }
        pData = HashAVLTree_EnumNext(pTable);
        if ( strcmp((char *)pData, "88") != 0 )
        {           
            printf( "HashAVLTree_EnumNext() ²âÊÔÓÃÀý4Ê§°Ü2!\n" );
        }
        pData = HashAVLTree_EnumNext(pTable);
        if ( strcmp((char *)pData, "100") != 0 )
        {           
            printf( "HashAVLTree_EnumNext() ²âÊÔÓÃÀý4Ê§°Ü3!\n" );
        }
        pData = HashAVLTree_EnumNext(pTable);
        if ( strcmp((char *)pData, "252") != 0 )
        {           
            printf( "HashAVLTree_EnumNext() ²âÊÔÓÃÀý4Ê§°Ü4!\n" );
        }
        pData = HashAVLTree_EnumNext(pTable);
        if ( strcmp((char *)pData, "728") != 0 )
        {           
            printf( "HashAVLTree_EnumNext() ²âÊÔÓÃÀý4Ê§°Ü5!\n" );
        }
        pData = HashAVLTree_EnumNext(pTable);
        if ( strcmp((char *)pData, "101") != 0 )
        {           
            printf( "HashAVLTree_EnumNext() ²âÊÔÓÃÀý4Ê§°Ü6!\n" );
        }
        pData = HashAVLTree_EnumNext(pTable);
        if ( pData != NULL )
        {           
            printf( "HashAVLTree_EnumNext() ²âÊÔÓÃÀý4Ê§°Ü7!\n" );
        }
        break;    

    default:
        break;
    }
    if ( pTable != NULL && pTable->uBucketCount != 0  )
    {
        HashAVLTree_Destroy(pTable, free);
    }
}


void TestCase_HashAVLTree_EnumNext(void)
{
    HASHAVLTREE *pTree;
    void *pData;

    pTree = HashAVLTree_Create(300);

    HashAVLTree_EnumBegin(pTree);
    pData = HashAVLTree_EnumNext(pTree);

    assertTrue(pTree->pCurEntry == NULL && pData == NULL);

    HashAVLTree_Insert(pTree, (void *)100, HashInt, IntCompare);

    HashAVLTree_EnumBegin(pTree);
    pData = HashAVLTree_EnumNext(pTree);

    assertTrue(pTree->pCurEntry == NULL && pData == (void *)100);

    pData = HashAVLTree_EnumNext(pTree);

    assertTrue(pTree->pCurEntry == NULL && pData == NULL);

    HashAVLTree_Insert(pTree, (void *)90, HashInt, IntCompare);

    HashAVLTree_EnumBegin(pTree);
    pData = HashAVLTree_EnumNext(pTree);

    assertTrue( pData == (void *)90);

    assertTrue(pTree->pCurEntry == NULL || pTree->pCurEntry->pData == (void *)100 );

    pData = HashAVLTree_EnumNext(pTree);

    assertTrue(pTree->pCurEntry == NULL && pData == (void *)100);

    HashAVLTree_Insert(pTree, (void *)70, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)80, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)60, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)50, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)40, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)30, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)20, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)10, HashInt, IntCompare);

    HashAVLTree_Insert(pTree, (void *)110, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)120, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)150, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)130, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)140, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)180, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)160, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)170, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)200, HashInt, IntCompare);
    HashAVLTree_Insert(pTree, (void *)190, HashInt, IntCompare);

    HashAVLTree_EnumBegin(pTree);
    for ( int i = 1; i < 20; i++ )
    {
        pData = HashAVLTree_EnumNext(pTree);

        assertTrue(pData == (void *)(i * 10) );

        assertTrue(pTree->pCurEntry == NULL || pTree->pCurEntry->pData == (void *)(i * 10 + 10));
    }

    pData = HashAVLTree_EnumNext(pTree);

    assertTrue(pData == (void *)(200) && pTree->pCurEntry == NULL);

    return;
}


void TestCase_HashAVLTree_EnumNextAndDelete(void)
{
    int i, k;
    HASHAVLTREE *pTree;
    AVLTREENODE *pCurNode;

    for ( i = 1; i <= 100; i++ )
    {
        pTree = HashAVLTree_Create(300);
        for ( k = 1; k <= 100; k++ )
        {
            HashAVLTree_Insert(pTree, (void *)k, HashInt, IntCompare);
        }
        pCurNode = HashAVLTree_FindNode(pTree, (void *)i, HashInt, IntCompare);
        if ( pCurNode == NULL )
        {
            printf("TestCase_AVLTreeNode_GetNext(): cannot find nod %d\n", i);
        }

        pTree->uCurBucketNo = HashInt((void *)i, 300);
        pTree->pCurEntry = pCurNode;

        HashAVLTree_Delete(pTree, (void *)i, HashInt, IntCompare, NULL);

        if ( i < 99 )
        {
            if ( pTree->pCurEntry != NULL )
            {
                int nData = (int)(pTree->pCurEntry->pData);
                assertTrue(nData == (i+1));
            }
            else
            {
                int ret = (int)HashAVLTree_EnumNext(pTree);
                if ( ret != (i+1) )
                {
                    printf("Error Check HashAVLTree_EnumNextAndDelete().\n");
                }
            }
        }
        else
        {
            void *pData = HashAVLTree_EnumNext(pTree);
            assertTrue(pTree->pCurEntry == NULL || pData == NULL);
        }

        HashAVLTree_Destroy(pTree, NULL);
    }

    return;
}
