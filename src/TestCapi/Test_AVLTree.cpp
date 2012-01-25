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
#include <stdio.h>
#include "TestCapi.h"
#include "CapiGlobal.h"
#include "TestApi.h"
#include "BinTree.h"
#include "AVLTree.h"
#include <stdlib.h>
#include <time.h>

int StrCompareNoCase( void *pStr1, void *pStr2 )
{
    return stricmp( (char *)pStr1, (char *)pStr2 );
}


void DRV_AVLTree_Delete(UINT i);
void DRV_AVLTree_Insert(UINT i);
void DRV_AVLTree_Destroy(UINT i);
void DRV_AVLTree_Find(UINT i);

void TestCase_AVLTree_RandomInsertAndDelete(void);

void TestCase_AVLTRee_InsertDelete_Special(void);
void TestCase_AVLTree_EnumNext(void);

void TestCase_AVLTree_GetNext(void);
void TestCase_AVLTree_EnumNextAndDelete(void);


void DRV_AVLTree_Composite();

void Test_AVLTree()
{
    int     i;
    for ( i = 1; i < 50; i++ )
    {
        DRV_AVLTree_Insert(i);
        DRV_AVLTree_Delete(i);
		DRV_AVLTree_Find(i);
		DRV_AVLTree_Destroy(i);
    }

 //   DRV_AVLTree_Composite();
 //   TestCase_Add(TestCase_AVLTree_RandomInsertAndDelete);

    TestCase_Add(TestCase_AVLTRee_InsertDelete_Special);

    TestCase_Add(TestCase_AVLTree_EnumNext);
    TestCase_Add(TestCase_AVLTree_GetNext);
    TestCase_Add(TestCase_AVLTree_EnumNextAndDelete);

    printf("Test_AVLTree()\n");
}

REGISTER_TESTFUNC(Test_AVLTree)


int AVLTree_CheckCount(AVLTREENODE *pStartNode)
{
    AVLTREENODE *pLeftNode, *pRightNode;
    UINT        uCount;
    if ( pStartNode == NULL )
    {
        return CAPI_FAILED;
    }

    uCount = AVLTree_GetSubTreeCount(pStartNode) + 1;

    assertEquals(uCount, pStartNode->uCount);
    if ( uCount != pStartNode->uCount )
    {
        printf("pStartNode->pData = %s\n", (char *)(pStartNode->pData));
    }
    pLeftNode = pStartNode->pLeft;
    pRightNode = pStartNode->pRight;

    AVLTree_CheckCount(pLeftNode);
    AVLTree_CheckCount(pRightNode);
    return CAPI_SUCCESS;
}


void DRV_AVLTree_Create(UINT i)
{
    AVLTREE *pTree;
    switch( i )
    {
    case 1:
        pTree = AVLTree_Create();
        if ( pTree != NULL && pTree->pRoot == NULL 
            && pTree->uNodeCount == 0 )
        {
            
        }
        else
        {
            printf( "AVLTree_Create() 测试用例失败!\n" );
        }
        AVLTree_Destroy(pTree, NULL);
        break;
    default:
        break;
    }
}


void DRV_AVLTree_Insert(UINT i)
{
    AVLTREE *pTree = AVLTree_Create();
    switch( i )
    {
    case 1:  /* 测试插入1个节点的情况 */
        AVLTree_Insert(pTree, strdup("Hello"), StrCompareNoCase);
        if ( strcmp((char *)(pTree->pRoot->pData), "Hello") == 0
            && pTree->pRoot->pLeft == NULL 
            && pTree->pRoot->pRight == NULL
            && pTree->pRoot->pParent == NULL 
            && pTree->pRoot->nMagic == 0 
            && pTree->uNodeCount == 1 )
        {
            //            printf( "Insert one node Successful\n" );
        }
        else
        {
            printf( "AVLTree_Insert()测试用例1 插入 失败\n" );
        }
        break;
    case 2: /* 测试插入2个节点的情况, 第2个节点大于第1个节点 */
        AVLTree_Insert(pTree, strdup("Hello"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("zhouweiming"), StrCompareNoCase);
        if ( strcmp((char *)(pTree->pRoot->pData), "Hello") == 0
            && pTree->pRoot->pLeft == NULL 
            && strcmp((char *)(pTree->pRoot->pRight->pData), "zhouweiming" ) == 0
            && pTree->pRoot->nMagic == -1 
            && pTree->pRoot->pRight->nMagic == 0 
            && pTree->pRoot->pRight->pLeft == NULL
            && pTree->pRoot->pRight->pRight == NULL
            && pTree->uNodeCount == 2 )
        {
            //            printf( "Insert Two node Successful\n" );
        }
        else
        {
            printf( "AVLTree_Insert()测试用例2 插入 失败\n" );
        }
        break;
    case 3: /* 测试插入2个节点的情况, 第2个节点小于第1个节点 */
        AVLTree_Insert(pTree, strdup("Hello"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("abcd"), StrCompareNoCase);
        if ( strcmp((char *)(pTree->pRoot->pData), "Hello") == 0
            && pTree->pRoot->pRight == NULL 
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "abcd" ) == 0
            && pTree->pRoot->nMagic == 1 
            && pTree->pRoot->pLeft->nMagic == 0 
            && pTree->pRoot->pLeft->pLeft == NULL
            && pTree->pRoot->pLeft->pRight == NULL
            && pTree->uNodeCount == 2 )
        {
            //            printf( "Insert Two node Successful\n" );
        }
        else
        {
            printf( "AVLTree_Insert()测试用例3 插入 失败\n" );
        }
        break;
    case 4:/* 测试插入3个节点的情况, LL型 */
        AVLTree_Insert(pTree, strdup("Hello"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("abcd"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("aaaa"), StrCompareNoCase);
        if ( strcmp((char *)(pTree->pRoot->pData), "abcd") == 0
            && strcmp((char *)(pTree->pRoot->pRight->pData), "Hello") == 0
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "aaaa" ) == 0
            && pTree->pRoot->nMagic == 0 
            && pTree->pRoot->pRight->nMagic == 0
            && pTree->pRoot->pRight->pLeft == NULL
            && pTree->pRoot->pRight->pRight == NULL
            && pTree->pRoot->pLeft->nMagic == 0 
            && pTree->pRoot->pLeft->pLeft == NULL
            && pTree->pRoot->pLeft->pRight == NULL 
            && pTree->uNodeCount == 3 )
        {
            //            printf( "Insert Two node Successful\n" );
        }
        else
        {
            printf( "AVLTree_Insert()测试用例4 插入 失败\n" );
        }
        break;
    case 5:/* 测试插入3个节点的情况, LR型 */
        AVLTree_Insert(pTree, strdup("Hello"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("abcd"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("bbbb"), StrCompareNoCase);
        if ( strcmp((char *)(pTree->pRoot->pData), "bbbb") == 0
            && strcmp((char *)(pTree->pRoot->pRight->pData), "Hello") == 0
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "abcd" ) == 0
            && pTree->pRoot->nMagic == 0 
            && pTree->pRoot->pRight->nMagic == 0
            && pTree->pRoot->pRight->pLeft == NULL
            && pTree->pRoot->pRight->pRight == NULL
            && pTree->pRoot->pLeft->nMagic == 0 
            && pTree->pRoot->pLeft->pLeft == NULL
            && pTree->pRoot->pLeft->pRight == NULL 
            && pTree->uNodeCount == 3 )
        {
            //            printf( "Insert Two node Successful\n" );
        }
        else
        {
            printf( "AVLTree_Insert()测试用例5 插入 失败\n" );
        }
        break;    
    case 6:/* 测试插入3个节点的情况, RL型 */
        AVLTree_Insert(pTree, strdup("aa"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("abcd"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("aaaa"), StrCompareNoCase);
        if ( strcmp((char *)(pTree->pRoot->pData), "aaaa") == 0
            && strcmp((char *)(pTree->pRoot->pRight->pData), "abcd") == 0
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "aa" ) == 0
            && pTree->pRoot->nMagic == 0 
            && pTree->pRoot->pRight->nMagic == 0
            && pTree->pRoot->pRight->pLeft == NULL
            && pTree->pRoot->pRight->pRight == NULL
            && pTree->pRoot->pLeft->nMagic == 0 
            && pTree->pRoot->pLeft->pLeft == NULL
            && pTree->pRoot->pLeft->pRight == NULL 
            && pTree->uNodeCount == 3 )
        {
            //            printf( "Insert Two node Successful\n" );
        }
        else
        {
            printf( "AVLTree_Insert()测试用例6 插入 失败\n" );
        }
        break;
    case 7:/* 测试插入3个节点的情况, RR型 */
        AVLTree_Insert(pTree, strdup("aa"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("abcd"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("bbbb"), StrCompareNoCase);
        if ( strcmp((char *)(pTree->pRoot->pData), "abcd") == 0
            && strcmp((char *)(pTree->pRoot->pRight->pData), "bbbb") == 0
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "aa" ) == 0
            && pTree->pRoot->nMagic == 0 
            && pTree->pRoot->pRight->nMagic == 0
            && pTree->pRoot->pRight->pLeft == NULL
            && pTree->pRoot->pRight->pRight == NULL
            && pTree->pRoot->pLeft->nMagic == 0 
            && pTree->pRoot->pLeft->pLeft == NULL
            && pTree->pRoot->pLeft->pRight == NULL 
            && pTree->uNodeCount == 3 )
        {
            //            printf( "Insert Two node Successful\n" );
        }
        else
        {
            printf( "AVLTree_Insert()测试用例7 插入 失败\n" );
        }
        break;    
    case 8:/* 测试插入9个节点的情况, LL型 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("17"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("24"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("13"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("15"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("23"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("10"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("1"), StrCompareNoCase);
        if ( strcmp((char *)(pTree->pRoot->pData), "20") == 0
            && strcmp((char *)(pTree->pRoot->pRight->pData), "24") == 0
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "15" ) == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pLeft->pData), "10") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pLeft->pLeft->pData), "1") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pLeft->pRight->pData), "13") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pRight->pData), "17") == 0
            && pTree->pRoot->nMagic == 1 
            && pTree->pRoot->pRight->nMagic == 0
            && pTree->pRoot->pRight->pLeft->pLeft == NULL
            && pTree->pRoot->pRight->pRight->pRight == NULL
            && pTree->pRoot->pLeft->nMagic == 1 
            && pTree->pRoot->pLeft->pLeft->pLeft->pLeft == NULL
            && pTree->pRoot->pLeft->pRight->pLeft == NULL 
            && pTree->pRoot->pLeft->pLeft->nMagic == 0
            && pTree->uNodeCount == 9 )
        {
            //            printf( "Insert Two node Successful\n" );
        }
        else
        {
            printf( "AVLTree_Insert()测试用例8 插入 失败\n" );
        }
        break;
    case 9:/* 测试插入9个节点的情况, A节点(13节点)平衡因子为1插入在右子树情况 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("17"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("24"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("13"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("15"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("23"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("10"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("14"), StrCompareNoCase);
        if ( strcmp((char *)(pTree->pRoot->pData), "20") == 0
            && strcmp((char *)(pTree->pRoot->pRight->pData), "24") == 0
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "15" ) == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pLeft->pData), "13") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pLeft->pLeft->pData), "10") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pLeft->pRight->pData), "14") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pRight->pData), "17") == 0
            && pTree->pRoot->nMagic == 1 
            && pTree->pRoot->pRight->nMagic == 0
            && pTree->pRoot->pRight->pLeft->pLeft == NULL
            && pTree->pRoot->pRight->pRight->pRight == NULL
            && pTree->pRoot->pRight->pParent == pTree->pRoot
            && pTree->pRoot->pLeft->nMagic == 1 
            && pTree->pRoot->pLeft->pLeft->pLeft->pLeft == NULL
            && pTree->pRoot->pLeft->pRight->pLeft == NULL 
            && pTree->pRoot->pLeft->pParent == pTree->pRoot
            && pTree->pRoot->pLeft->pLeft->nMagic == 0
            && pTree->uNodeCount == 9 )
        {
            //            printf( "Insert Two node Successful\n" );
        }
        else
        {
            printf( "AVLTree_Insert()测试用例9 插入 失败\n" );
        }
        break;
    case 10:/* 测试插入9个节点的情况, LR型 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("17"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("24"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("13"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("15"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("23"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("10"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("11"), StrCompareNoCase);
        if ( strcmp((char *)(pTree->pRoot->pData), "20") == 0
            && strcmp((char *)(pTree->pRoot->pRight->pData), "24") == 0
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "15" ) == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pLeft->pData), "11") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pLeft->pLeft->pData), "10") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pLeft->pRight->pData), "13") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pRight->pData), "17") == 0
            && pTree->pRoot->nMagic == 1 
            && pTree->pRoot->pRight->nMagic == 0
            && pTree->pRoot->pRight->pLeft->pLeft == NULL
            && pTree->pRoot->pRight->pRight->pRight == NULL
            && pTree->pRoot->pLeft->nMagic == 1 
            && pTree->pRoot->pLeft->pLeft->pLeft->pLeft == NULL
            && pTree->pRoot->pLeft->pRight->pLeft == NULL 
            && pTree->pRoot->pLeft->pLeft->nMagic == 0
            && pTree->uNodeCount == 9 )
        {
            //            printf( "Insert Two node Successful\n" );
        }
        else
        {
            printf( "AVLTree_Insert()测试用例10 插入 失败\n" );
        }
        break;
    case 11:/* 测试插入9个节点的情况, RL型 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("17"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("24"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("13"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("15"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("23"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("19"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("18"), StrCompareNoCase);
        if ( strcmp((char *)(pTree->pRoot->pData), "20") == 0
            && strcmp((char *)(pTree->pRoot->pRight->pData), "24") == 0
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "15" ) == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pLeft->pData), "13") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pRight->pRight->pData), "19") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pRight->pLeft->pData), "17") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pRight->pData), "18") == 0
            && pTree->pRoot->nMagic == 1 
            && pTree->pRoot->pRight->nMagic == 0
            && pTree->pRoot->pRight->pLeft->pLeft == NULL
            && pTree->pRoot->pRight->pRight->pRight == NULL
            && pTree->pRoot->pLeft->nMagic == -1 
            && pTree->pRoot->pLeft->pLeft->pLeft == NULL
            && pTree->pRoot->pLeft->pLeft->nMagic == 0
            && pTree->uNodeCount == 9 )
        {
            //            printf( "Insert Two node Successful\n" );
        }
        else
        {
            printf( "AVLTree_Insert()测试用例11 插入 失败\n" );
        }
        break;
    case 12:/* 测试插入9个节点的情况, RR型 */
        AVLTree_Insert(pTree, strdup("21"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("17"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("24"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("13"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("15"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("23"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("19"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        if ( strcmp((char *)(pTree->pRoot->pData), "21") == 0
            && strcmp((char *)(pTree->pRoot->pRight->pData), "24") == 0
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "15" ) == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pLeft->pData), "13") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pRight->pRight->pData), "20") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pRight->pLeft->pData), "17") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pRight->pData), "19") == 0
            && pTree->pRoot->nMagic == 1 
            && pTree->pRoot->pRight->nMagic == 0
            && pTree->pRoot->pRight->pLeft->pLeft == NULL
            && pTree->pRoot->pRight->pRight->pRight == NULL
            && pTree->pRoot->pLeft->nMagic == -1 
            && pTree->pRoot->pLeft->pRight->nMagic == 0
            && pTree->pRoot->pLeft->pRight->pLeft->nMagic == 0
            && pTree->pRoot->pLeft->pLeft->pLeft == NULL
            && pTree->pRoot->pLeft->pLeft->nMagic == 0
            && pTree->uNodeCount == 9 )
        {
            //            printf( "Insert Two node Successful\n" );
        }
        else
        {
            printf( "AVLTree_Insert()测试用例12 插入 失败\n" );
        }
        break;
    case 13:/* 测试插入11个节点的情况, A节点平衡因子为-1, 插入在左子树的情况 */
        AVLTree_Insert(pTree, strdup("21"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("17"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("24"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("13"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("15"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("23"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("19"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("12"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("14"), StrCompareNoCase);
        if ( strcmp((char *)(pTree->pRoot->pData), "21") == 0
            && strcmp((char *)(pTree->pRoot->pRight->pData), "24") == 0
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "15" ) == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pLeft->pData), "13") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pRight->pRight->pData), "20") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pRight->pLeft->pData), "17") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pRight->pData), "19") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pLeft->pLeft->pData), "12") == 0
            && strcmp( (char *)(pTree->pRoot->pLeft->pLeft->pRight->pData), "14") == 0
            && pTree->pRoot->nMagic == 1 
            && pTree->pRoot->pRight->nMagic == 0
            && pTree->pRoot->pRight->pLeft->pLeft == NULL
            && pTree->pRoot->pRight->pRight->pRight == NULL
            && pTree->pRoot->pLeft->nMagic == 0 
            && pTree->pRoot->pLeft->pRight->nMagic == 0
            && pTree->pRoot->pLeft->pRight->pLeft->nMagic == 0
            && pTree->pRoot->pLeft->pLeft->pLeft->pLeft == NULL
            && pTree->pRoot->pLeft->pLeft->nMagic == 0
            && pTree->uNodeCount == 11 )
        {
            //            printf( "Insert Two node Successful\n" );
        }
        else
        {
            printf( "AVLTree_Insert()测试用例12 插入 失败\n" );
        }
        break;
    default:
        break;
    }
    if ( pTree->pRoot != NULL )
    {
        if (pTree->pRoot->pParent != NULL )
        {
            printf( "Root Node's Parent Node is not NULL\n" );
        }
    }
    BinTree_CheckParent(pTree->pRoot);

    AVLTree_CheckCount(pTree->pRoot);

    AVLTree_Destroy(pTree, free);
}

void DRV_AVLTree_Delete(UINT i)
{
    AVLTREE *pTree = AVLTree_Create();
    switch( i )
    {
    case 1: /* 测试树为空时的删除情况 */
        AVLTree_Delete(pTree, (void *)"hELLO", StrCompareNoCase, free );
        break;
    case 2: /* 测试插入一个节点，删除一个节点的情况 */
        AVLTree_Insert(pTree, strdup("Hello"), StrCompareNoCase);
        AVLTree_Delete(pTree, (void *)"Hello", StrCompareNoCase, free);
        if ( pTree->pRoot == NULL
            && pTree->uNodeCount == 0 )
        {
        }
        else
        {
            printf( "AVLTree_Delete()测试用例2 删除 失败\n" );
        }
        break;
    case 3: /* 测试插入两个节点，删除根节点的情况 */
        AVLTree_Insert(pTree, strdup("Hello"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("zhouweiming"), StrCompareNoCase);

        AVLTree_Delete(pTree, (void *)"Hello", StrCompareNoCase, free);
        if ( strcmp((char *)(pTree->pRoot->pData), "zhouweiming") == 0 
            && pTree->pRoot->nMagic == 0 
            && pTree->pRoot->pLeft == NULL 
            && pTree->pRoot->pRight == NULL 
            && pTree->pRoot->pParent == NULL
            &&pTree->uNodeCount == 1 )
        {
        }
        else
        {
            printf( "AVLTree_Delete()测试用例3 删除 失败\n" );
        }
        break;
    case 4: /* 测试插入两个节点，删除非根节点的情况 */
        AVLTree_Insert(pTree, strdup("Hello"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("zhouweiming"), StrCompareNoCase);

        AVLTree_Delete(pTree, (void *)"zhouweiming", StrCompareNoCase, free);
        if ( strcmp((char *)(pTree->pRoot->pData), "Hello") == 0 
            && pTree->pRoot->nMagic == 0 
            && pTree->pRoot->pLeft == NULL 
            && pTree->pRoot->pRight == NULL 
            && pTree->pRoot->pParent == NULL 
            && pTree->uNodeCount == 1 )
        {
//            printf( "Delete \"Hello\" Node Successful\n" );
        }
        else
        {
            printf( "AVLTree_Delete()测试用例4 删除 失败\n" );
        }
        break;
    case 5: /* 测试L0型情况 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("18"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("27"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("22"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);

        AVLTree_Delete(pTree, (void *)"18", StrCompareNoCase, free);
        if ( strcmp((char *)(pTree->pRoot->pData), "27") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "20") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pData), "28") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pRight->pData), "22") == 0 
            && pTree->pRoot->nMagic == 1 && pTree->pRoot->pLeft->nMagic == -1
            && pTree->pRoot->pRight->nMagic == 0 
            && pTree->pRoot->pLeft->pRight->nMagic == 0 
            && pTree->uNodeCount == 4 )
        {

        }
        else
        {
            printf( "AVLTree_Delete()测试用例5 L0型不平衡测试 删除 失败\n" );
        }
        break;
    case 6: /* 测试L-1型情况 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("18"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("27"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
        
        AVLTree_Delete(pTree, (void *)"18", StrCompareNoCase, free);
        if ( strcmp((char *)(pTree->pRoot->pData), "27") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "20") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pData), "28") == 0 
            && pTree->pRoot->nMagic == 0 && pTree->pRoot->pLeft->nMagic == 0
            && pTree->pRoot->pRight->nMagic == 0 
            && pTree->uNodeCount == 3 )
        {
            
        }
        else
        {
            printf( "AVLTree_Delete()测试用例6 L-1型不平衡测试 删除 失败\n" );
        }
        break;
    case 7: /* 测试L1型情况 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("18"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("27"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("25"), StrCompareNoCase);
        
        AVLTree_Delete(pTree, (void *)"18", StrCompareNoCase, free);
        if ( strcmp((char *)(pTree->pRoot->pData), "25") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "20") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pData), "27") == 0 
            && pTree->pRoot->nMagic == 0 && pTree->pRoot->pLeft->nMagic == 0
            && pTree->pRoot->pRight->nMagic == 0 
            && pTree->uNodeCount == 3 )
        {
            
        }
        else
        {
            printf( "AVLTree_Delete()测试用例7 L1型不平衡测试 删除 失败\n" );
        }
        break;
    case 8: /* 测试R0型情况 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("17"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("18"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("12"), StrCompareNoCase);
        
        AVLTree_Delete(pTree, (void *)"28", StrCompareNoCase, free);
        if ( strcmp((char *)(pTree->pRoot->pData), "17") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "12") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pData), "20") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pLeft->pData), "18") == 0 
            && pTree->pRoot->nMagic == -1 && pTree->pRoot->pLeft->nMagic == 0
            && pTree->pRoot->pRight->nMagic == 1 
            && pTree->uNodeCount == 4 )
        {
            
        }
        else
        {
            printf( "AVLTree_Delete()测试用例8 R0型不平衡测试 删除 失败\n" );
        }
        break;
    case 9: /* 测试R-1型情况 */
        break;
    case 10: /* 测试R1型情况 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("17"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("12"), StrCompareNoCase);
        
        AVLTree_Delete(pTree, (void *)"28", StrCompareNoCase, free);
        if ( strcmp((char *)(pTree->pRoot->pData), "17") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "12") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pData), "20") == 0 
            && pTree->pRoot->nMagic == 0 && pTree->pRoot->pLeft->nMagic == 0
            && pTree->pRoot->pRight->nMagic == 0 
            && pTree->uNodeCount == 3 )
        {
            
        }
        else
        {
            printf( "AVLTree_Delete()测试用例10 R1型不平衡测试 删除 失败\n" );
        }
        break;
    case 11: /* 测试R-1型情况 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("17"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("18"), StrCompareNoCase);
        
        AVLTree_Delete(pTree, (void *)"28", StrCompareNoCase, free);
        if ( strcmp((char *)(pTree->pRoot->pData), "18") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "17") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pData), "20") == 0 
            && pTree->pRoot->nMagic == 0 && pTree->pRoot->pLeft->nMagic == 0
            && pTree->pRoot->pRight->nMagic == 0 
            && pTree->uNodeCount == 3 )
        {
        }
        else
        {
            printf( "AVLTree_Delete()测试用例11 R-1型不平衡测试 删除 失败\n" );
        }
        break;    
    case 12: /* 测试R-1型情况, C节点平衡因子为1  */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("16"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("29"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("15"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("19"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("17"), StrCompareNoCase);
        
        AVLTree_Delete(pTree, (void *)"28", StrCompareNoCase, free);
        if ( strcmp((char *)(pTree->pRoot->pData), "19") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "16") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pData), "20") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pLeft->pData), "15") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pRight->pData), "29") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pRight->pData), "17") == 0 
            && pTree->pRoot->nMagic == 0 && pTree->pRoot->pRight->nMagic == -1
            && pTree->pRoot->pLeft->nMagic == 0 
            && pTree->pRoot->pLeft->pLeft->nMagic == 0
            && pTree->pRoot->pRight->pRight->nMagic == 0
            && pTree->pRoot->pLeft->pRight->nMagic == 0
            && pTree->uNodeCount == 6 )
        {
        }
        else
        {
            printf( "AVLTree_Delete()测试用例12 R-1型不平衡测试 删除 失败\n" );
        }
        break;    
    case 13: /* 测试R-1型情况, C节点平衡因子为－1 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("16"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("29"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("15"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("17"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("19"), StrCompareNoCase);
        
        AVLTree_Delete(pTree, (void *)"28", StrCompareNoCase, free);
        if ( strcmp((char *)(pTree->pRoot->pData), "17") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "16") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pData), "20") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pLeft->pData), "15") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pRight->pData), "29") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pLeft->pData), "19") == 0 
            && pTree->pRoot->nMagic == 0 && pTree->pRoot->pRight->nMagic == 0
            && pTree->pRoot->pLeft->nMagic == 1 
            && pTree->pRoot->pLeft->pLeft->nMagic == 0
            && pTree->pRoot->pRight->pRight->nMagic == 0
            && pTree->pRoot->pRight->pLeft->nMagic == 0
            && pTree->uNodeCount == 6 )
        {
        }
        else
        {
            printf( "AVLTree_Delete()测试用例13 R1型不平衡测试 删除 失败\n" );
        }
        break;
    case 14: /* 复合情况1 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("14"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("25"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("13"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("18"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("22"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("12"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("17"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("19"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("21"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("23"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("29"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("16"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("24"), StrCompareNoCase);
        
        AVLTree_Delete(pTree, (void *)"12", StrCompareNoCase, free);
        if ( strcmp((char *)(pTree->pRoot->pData), "20") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "17") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pData), "25") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pLeft->pData), "14") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pRight->pData), "18") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pLeft->pLeft->pData), "13") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pLeft->pRight->pData), "16") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pRight->pRight->pData), "19") == 0 
            && pTree->pRoot->nMagic == -1 && pTree->pRoot->pRight->nMagic == 1
            && pTree->pRoot->pLeft->nMagic == 0 
            && pTree->pRoot->pLeft->pLeft->nMagic == 0
            && pTree->pRoot->pLeft->pRight->nMagic == -1
            && pTree->pRoot->pLeft->pLeft->pRight->nMagic == 0
            && pTree->pRoot->pLeft->pLeft->pLeft->nMagic == 0
            && pTree->pRoot->pLeft->pRight->pRight->nMagic == 0
            && pTree->uNodeCount == 14 )
        {
        }
        else
        {
            printf( "AVLTree_Delete()测试用例14 复合型1不平衡测试 删除 失败\n" );
        }
        break;
    case 15: /* 复合情况2，删除节点的左右节点都存在的情况 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("14"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("25"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("13"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("18"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("22"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("12"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("17"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("19"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("21"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("23"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("29"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("16"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("24"), StrCompareNoCase);
        
        AVLTree_Delete(pTree, (void *)"25", StrCompareNoCase, free);
        if ( strcmp((char *)(pTree->pRoot->pData), "20") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "14") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pData), "24") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pLeft->pData), "22") == 0 
            && pTree->pRoot->nMagic == 1 && pTree->pRoot->pRight->nMagic == 0
            && pTree->pRoot->pLeft->nMagic == -1 
            && pTree->pRoot->pRight->pRight->nMagic == -1
            && pTree->uNodeCount == 14 )
        {
        }
        else
        {
            printf( "AVLTree_Delete()测试用例15 复合型2不平衡测试 删除 失败\n" );
        }
        break;
    case 16: /* 2个节点的平衡测试 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("18"), StrCompareNoCase);
        AVLTree_Delete(pTree, (void *)"18", StrCompareNoCase, free);

        if ( strcmp((char *)(pTree->pRoot->pData), "20") == 0 
            && pTree->pRoot->pLeft == NULL 
            && pTree->pRoot->pRight == NULL 
            && pTree->pRoot->nMagic == 0 ) 
        {
        }
        else
        {
            printf( "AVLTree_Delete()测试用例16 2个节点的平衡测试 删除 失败\n" );
        }
        break;
    case 17: /* 4个节点的平衡测试 */
        AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("18"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("22"), StrCompareNoCase);
        AVLTree_Insert(pTree, strdup("15"), StrCompareNoCase);
        AVLTree_Delete(pTree, (void *)"15", StrCompareNoCase, free);
        
        if ( strcmp((char *)(pTree->pRoot->pData), "20") == 0 
            && strcmp((char *)(pTree->pRoot->pLeft->pData), "18") == 0 
            && strcmp((char *)(pTree->pRoot->pRight->pData), "22") == 0 
            && pTree->pRoot->pLeft->pLeft == NULL
            && pTree->pRoot->pLeft->pRight == NULL 
            && pTree->pRoot->pRight->pLeft == NULL 
            && pTree->pRoot->pRight->pRight == NULL
            && pTree->pRoot->nMagic == 0 
            && pTree->pRoot->pLeft->nMagic == 0 
            && pTree->pRoot->pRight->nMagic == 0 ) 
        {
        }
        else
        {
            printf( "AVLTree_Delete()测试用例17 4个节点的平衡测试 删除 失败\n" );
        }
        break;
    default:
        break;
    }
    BinTree_CheckParent(pTree->pRoot);
    AVLTree_CheckCount(pTree->pRoot);
    if ( pTree->pRoot != NULL )
    {
        if (pTree->pRoot->pParent != NULL )
        {
            printf( "Root Node's Parent Node is not NULL\n" );
        }
    }
    AVLTree_Destroy(pTree, free);
}

void DRV_AVLTree_Find(UINT i)
{
	char *pszTemp;
    AVLTREE *pTree = AVLTree_Create();
    AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
    AVLTree_Insert(pTree, strdup("16"), StrCompareNoCase);
    AVLTree_Insert(pTree, strdup("28"), StrCompareNoCase);
    AVLTree_Insert(pTree, strdup("29"), StrCompareNoCase);
    AVLTree_Insert(pTree, strdup("15"), StrCompareNoCase);
    AVLTree_Insert(pTree, strdup("17"), StrCompareNoCase);
    AVLTree_Insert(pTree, strdup("19"), StrCompareNoCase);

    switch( i )
    {
    case 1:
		pszTemp = (char *)AVLTree_Find(pTree, (void *)"20", StrCompareNoCase);
		if ( strcmp(pszTemp, "20") != 0 )
		{
			printf("AVLTree_Find()测试用例1 失败\n");
		}
        break;
    case 2:
		pszTemp = (char *)AVLTree_Find(pTree, (void *)"28", StrCompareNoCase);
		if ( strcmp(pszTemp, "28") != 0 )
		{
			printf("AVLTree_Find()测试用例2 失败\n");
		}
        break;
    case 3:
		pszTemp = (char *)AVLTree_Find(pTree, (void *)"16", StrCompareNoCase);
		if ( strcmp(pszTemp, "16") != 0 )
		{
			printf("AVLTree_Find()测试用例3 失败\n");
		}
        break;
    case 4:
		pszTemp = (char *)AVLTree_Find(pTree, (void *)"29", StrCompareNoCase);
		if ( strcmp(pszTemp, "29") != 0 )
		{
			printf("AVLTree_Find()测试用例4 失败\n");
		}
        break;
    case 5:
		pszTemp = (char *)AVLTree_Find(pTree, (void *)"15", StrCompareNoCase);
		if ( strcmp(pszTemp, "15") != 0 )
		{
			printf("AVLTree_Find()测试用例5 失败\n");
		}
        break;    
	case 6:
		pszTemp = (char *)AVLTree_Find(pTree, (void *)"17", StrCompareNoCase);
		if ( strcmp(pszTemp, "17") != 0 )
		{
			printf("AVLTree_Find()测试用例6 失败\n");
		}
        break;    
	case 7:
		pszTemp = (char *)AVLTree_Find(pTree, (void *)"19", StrCompareNoCase);
		if ( strcmp(pszTemp, "19") != 0 )
		{
			printf("AVLTree_Find()测试用例7 失败\n");
		}
        break;    
	case 8:
		AVLTree_Delete(pTree, (void *)"17", StrCompareNoCase, free);
		pszTemp = (char *)AVLTree_Find(pTree, (void *)"17", StrCompareNoCase);
		if ( pszTemp != NULL )
		{
			printf("AVLTree_Find()测试用例8 失败\n");
		}
        break;    		
	case 9:
		AVLTree_Delete(pTree, (void *)"17", StrCompareNoCase, free);
		AVLTree_Delete(pTree, (void *)"20", StrCompareNoCase, free);
        AVLTree_CheckCount(pTree->pRoot);
		AVLTree_Delete(pTree, (void *)"16", StrCompareNoCase, free);
		AVLTree_Delete(pTree, (void *)"15", StrCompareNoCase, free);
        AVLTree_CheckCount(pTree->pRoot);
		AVLTree_Delete(pTree, (void *)"29", StrCompareNoCase, free);

		pszTemp = (char *)AVLTree_Find(pTree, (void *)"28", StrCompareNoCase);
		if ( strcmp(pszTemp, "28") != 0 )
		{
			printf("AVLTree_Find()测试用例9 失败\n");
		}
        break;    		
	default:
        break;
    }
    AVLTree_CheckCount(pTree->pRoot);
	AVLTree_Destroy(pTree, free);
}

void DRV_AVLTree_Destroy(UINT i)
{
	AVLTREE *pTree = AVLTree_Create();
    switch( i )
    {
    case 1:
		AVLTree_Destroy(pTree, free);
        break;
    case 2:
		AVLTree_Destroy(pTree, NULL);
        break;
    case 3:
		AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
		AVLTree_Destroy(pTree, free);
        break;
    case 4:
		AVLTree_Insert(pTree, strdup("16"), StrCompareNoCase);
		AVLTree_Destroy(pTree, free);
        break;
    case 5:
		AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
		AVLTree_Insert(pTree, strdup("16"), StrCompareNoCase);
		AVLTree_Destroy(pTree, free);
        break;
    case 6:
		AVLTree_Insert(pTree, strdup("20"), StrCompareNoCase);
		AVLTree_Insert(pTree, strdup("16"), StrCompareNoCase);
		AVLTree_Insert(pTree, strdup("18"), StrCompareNoCase);
		AVLTree_Destroy(pTree, free);
        break;
    default:
        AVLTree_Destroy(pTree, NULL);
        break;
    }
}

int AVLTree_Visit(void *pData)
{
    printf("%d->\n", *(int *)pData);

    return CAPI_SUCCESS;
}

int PrintString(void *pData)
{
    printf("Error: %s\n", (char *)pData);

    return CAPI_SUCCESS;
}

int IntCompare2(void *p1, void *p2)
{
    int n1, n2;
    n1 = *(int *)p1;
    n2 = *(int *)p2;

    if ( n1 > n2 )
    {
        return 1;
    }
    else if ( n1 < n2 )
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void DRV_AVLTree_Composite()
{
    char szBuf[512];
    int nID;
    int i;
    int nRet;
    int nCheckRet;
    int nTestCount;
    int nCount;
    int *pN = NULL;
    char *psz;

    bool bIsString = true;

    pN = new int(0);

	AVLTREE *pTree = AVLTree_Create();
    srand(time(NULL));

    nTestCount = 10001;

    nID = 0;
    nCount = 0;

    for ( i = 0; i < nTestCount; i++ )
    {
        nID = i;
        sprintf(szBuf, "%s-%d", "KRRole1", nID);
        // sprintf(szBuf, "%d", nID);

        psz = strdup(szBuf);
        //puts(psz);

        if ( bIsString )
        {
            nRet = AVLTree_Insert(pTree, psz, StrCompareNoCase);
            nCheckRet = AVLTree_CheckMagic(pTree, PrintString);
        }
        else
        {
            nRet = AVLTree_Insert(pTree, (void *)(new int(nID)), IntCompare2);
            nCheckRet = AVLTree_CheckMagic(pTree, NULL);
        }

        if ( nRet == CAPI_SUCCESS )
        {
            nCount++;
        }
        else
        {
            printf("Insert %s failed\n", szBuf);
        }

        //printf("Inserting %s\n", psz);
        //BinTree_Print((BINTREE *)pTree);
        //BinTree_PrintMagic((BINTREE *)pTree);
        //puts("----------------------------------------");

        if ( nCheckRet == 0 )
        {
            printf("Error!\n");
            //BinTree_Print((BINTREE *)pTree);
            //BinTree_PrintMagic((BINTREE *)pTree);
            goto label_failed;
        }
    }

    printf("Insert Count = %d\n", nCount);

    puts("--------------------");
    puts("Inserting complete!");
    puts("--------------------");

    if ( bIsString )
    {
        nCheckRet = AVLTree_CheckMagic(pTree, PrintString);
    }
    else
    {
        BinTree_InOrderTraverse((BINTREE *)pTree, AVLTree_Visit);
        nCheckRet = AVLTree_CheckMagic(pTree, NULL);
    }

    if ( nCheckRet == 0 )
    {
        printf("Error!\n");
    }

    nCount = 0;
    for ( i = 0; i < nTestCount; i++ )
    {
        nID = nTestCount - i - 1;
        //nID = rand() % 100000;
        nID = GetTickCount() % 10001;

        sprintf(szBuf, "%s-%d", "KRRole1", nID);
        //printf("Deleting %s\n", szBuf);


        if ( bIsString )
        {
            nRet = AVLTree_Delete(pTree, szBuf, StrCompareNoCase, free);
            nCheckRet = AVLTree_CheckMagic(pTree, PrintString);
        }
        else
        {
            *pN = nID;
            nRet = AVLTree_Delete(pTree, (void *)pN, IntCompare2, free);
            nCheckRet = AVLTree_CheckMagic(pTree, NULL);
        }

        if ( nRet == CAPI_SUCCESS )
        {
            nCount++;
        }
        else
        {
            printf("Delete %s failed\n", szBuf);
        }

        if ( nCheckRet == 0 )
        {
            printf("Error!\n");
            goto label_failed;
        }

    }

    printf("Delete Count = %d\n", nCount);
    puts("--------------------");
    puts("Deleting complete!");
    puts("--------------------");

label_failed:
    if ( bIsString )
    {
        AVLTree_Destroy(pTree, free);
    }
    else
    {
        AVLTree_Destroy(pTree, NULL);
    }
    pTree = NULL;

    delete pN;
    pN = NULL;
}



void GetRandomArray(int nMaxDataValue, int *pnData, int nDataCount)
{
    int i;
    int k;
    int nData;

    /* 随机生成nCount个小于10000的整数 */
    for ( i = 0; i < nDataCount; i++ )
    {
        for ( ;; )
        {
            int nFindFlag = 0;
            int nDataTemp = GetTickCount();
            if ( nDataTemp < 0 )
            {
                nDataTemp = 0 - nDataTemp;
            }
            nData = nDataTemp % nMaxDataValue;

            if ( nData < 0 )
            {
                nData = 0 - nData;
            }

            for ( k = 0; k < i; k++ )
            {
                if ( nData == pnData[k] )
                {
                    nFindFlag = 1;
                    break;
                }
            }

            if ( nFindFlag == 0 )
            {
                pnData[i] = nData;
                break;
            }
            else
            {
                continue;
            }
        }
    }

    return;
}


INT PrintInt(void *pData)
{
    printf("pData = %ld\n", (INT)pData);
    return 0;
}


int AVLTree_Random_InsertDelete(int nCount)
{
    AVLTREE *pTree;
    int pnData[200];
    int pnData2[200];
    int pnOrder[200];
    int i;

 //   pnData = new int[nCount];
 //   pnOrder = new int[nCount];
 //   pnData2 = new int[nCount];

    pTree = AVLTree_Create();

    /* 随机生成nCount个小于10000的整数 */
    GetRandomArray(10000, pnData, nCount);
    GetRandomArray(nCount, pnOrder, nCount);

    for ( i = 0; i < nCount; i++ )
    {
        if ( pnData[i] == 0 )
        {
            pnData[i] = 10005;
        }
    }


    /* 插入数据 */
    for ( i = 0; i < nCount; i++ )
    {
        AVLTree_Insert(pTree, (void *)(pnData[i]), IntCompare);
    }

    /* 将pnData数组内的数据随机扰乱,输出到pnData2中 */
    for ( i = 0; i < nCount; i++ )
    {
        pnData2[i] = pnData[pnOrder[i]];
    }

    for ( i = 0; i < nCount; i++ )
    {
        AVLTree_Delete(pTree, (void *)(pnData2[i]), IntCompare, NULL);

        AVLTree_CheckMagic(pTree, PrintInt);
        if (pTree->uNodeCount != (nCount - i-1))
        {
            printf("pTree->uNodeCount = %d, i = %d\n", pTree->uNodeCount, i);

            printf("pnData[%d] = ", nCount);
            for ( int k = 0; k < nCount; k++ )
            {
                printf("%d, ", pnData[k]);
            }
            printf("\n");

            printf("pnData2[%d] = ", nCount);
            for ( int k = 0; k < nCount; k++ )
            {
                printf("%d, ", pnData2[k]);
            }
            printf("\n");
    

        }
    }


    assertTrue(pTree->uNodeCount == 0 && pTree->pRoot == NULL );

  //  delete pnData;
 //   delete pnData2;
  //  delete pnOrder;

    return CAPI_SUCCESS;
}




void TestCase_AVLTree_RandomInsertAndDelete(void)
{
    int i;
    int k;
    int anData[7];

    GetRandomArray(7, anData, 7);

#pragma omp parallel for num_threads(8)
    for ( i = 0; i < 200; i++ )
    {
        for ( k = 1; k < 100; k++ )
        {
            AVLTree_Random_InsertDelete(k);

            printf("k = %d, AVLTree_Random_InsertDelete() finished.\n", k);
        }

        printf ( "On Turn Test Finished. i = %ld\n", i);
    }

    printf("TestCase_AVLTree_RandomInsertAndDelete().\n");
    return;
}



void TestCase_AVLTRee_InsertDelete_Special(void)
{
    AVLTREE *pTree;
    int i;

    int pnData[47] = { 8771, 8505, 8267, 7980, 7756, 7476, 7203, 6916, 6643, 6342, 6027, 5691, 5383, 
        5040, 4711, 4361, 4018, 3647, 3276, 2884, 2499, 2086, 1694, 1267, 861, 420, 10005, 9552, 9111, 
        8642, 8180, 7683, 7221, 6724, 6234, 5709, 5205, 4673, 4155, 3616, 3084, 2510, 1957, 1376, 809, 207, 9570 };
    int pnData2[47] = { 5383, 1267, 2510, 861, 6916, 809, 5040, 8505, 1376, 10005, 3084, 8771,
        2086, 9111, 4711, 6724, 2884, 207, 3647, 7980, 9552, 3276, 6342, 7683, 6643, 7756, 
        4155, 9570, 3616, 1957, 8642, 4673, 4361, 1694, 8180, 6027, 5709, 6234, 7203, 7221, 8267, 5691, 7476, 5205, 2499, 4018, 420 };


    pTree = AVLTree_Create();

    for ( i = 0; i < 47; i++ )
    {
        AVLTree_Insert(pTree, (void *)(pnData[i]), IntCompare);
        AVLTree_CheckMagic(pTree, PrintInt);

        if ( i == 25 )
        {
            printf( "i = %d\n", i);
        }

        if ( pTree->uNodeCount != (i + 1) )
        {
            printf("Insert: pTree->uNodeCount = %d is not correct. i = %d \n", pTree->uNodeCount, i);
        }
    }


    for ( i = 0; i < 47; i++ )
    {
        AVLTree_Delete(pTree, (void *)(pnData2[i]), IntCompare, NULL);
        AVLTree_CheckMagic(pTree, PrintInt);
        if ( pTree->uNodeCount != 47 - (i + 1) )
        {
            printf("Delete: pTree->uNodeCount = %d is not correct. i = %d \n", pTree->uNodeCount, i);
        }
    }


    AVLTree_Destroy(pTree, NULL);

    return;

}


void TestCase_AVLTree_EnumNext(void)
{
    AVLTREE *pTree;
    void *pData;

    pTree = AVLTree_Create();

    AVLTree_EnumBegin(pTree);
    pData = AVLTree_EnumNext(pTree);

    assertTrue(pTree->pCursor == NULL && pData == NULL);

    AVLTree_Insert(pTree, (void *)100, IntCompare);

    AVLTree_EnumBegin(pTree);
    pData = AVLTree_EnumNext(pTree);

    assertTrue(pTree->pCursor == NULL && pData == (void *)100);

    pData = AVLTree_EnumNext(pTree);

    assertTrue(pTree->pCursor == NULL && pData == NULL);

    AVLTree_Insert(pTree, (void *)90, IntCompare);

    AVLTree_EnumBegin(pTree);
    pData = AVLTree_EnumNext(pTree);

    assertTrue(pTree->pCursor->pData == (void *)100 && pData == (void *)90);

    pData = AVLTree_EnumNext(pTree);

    assertTrue(pTree->pCursor == NULL && pData == (void *)100);

    AVLTree_Insert(pTree, (void *)70, IntCompare);
    AVLTree_Insert(pTree, (void *)80, IntCompare);
    AVLTree_Insert(pTree, (void *)60, IntCompare);
    AVLTree_Insert(pTree, (void *)50, IntCompare);
    AVLTree_Insert(pTree, (void *)40, IntCompare);
    AVLTree_Insert(pTree, (void *)30, IntCompare);
    AVLTree_Insert(pTree, (void *)20, IntCompare);
    AVLTree_Insert(pTree, (void *)10, IntCompare);

    AVLTree_Insert(pTree, (void *)110, IntCompare);
    AVLTree_Insert(pTree, (void *)120, IntCompare);
    AVLTree_Insert(pTree, (void *)150, IntCompare);
    AVLTree_Insert(pTree, (void *)130, IntCompare);
    AVLTree_Insert(pTree, (void *)140, IntCompare);
    AVLTree_Insert(pTree, (void *)180, IntCompare);
    AVLTree_Insert(pTree, (void *)160, IntCompare);
    AVLTree_Insert(pTree, (void *)170, IntCompare);
    AVLTree_Insert(pTree, (void *)200, IntCompare);
    AVLTree_Insert(pTree, (void *)190, IntCompare);

    AVLTree_EnumBegin(pTree);
    for ( int i = 1; i < 20; i++ )
    {
        pData = AVLTree_EnumNext(pTree);

        assertTrue(pData == (void *)(i * 10) && pTree->pCursor->pData == (void *)(i* 10 + 10));
    }

    pData = AVLTree_EnumNext(pTree);

    assertTrue(pData == (void *)(200) && pTree->pCursor == NULL);

    return;
}



void TestCase_AVLTree_GetNext(void)
{
    int i;
    AVLTREE *pTree;
    AVLTREENODE *pCurNode;
    AVLTREENODE *pNode;

    pTree = AVLTree_Create();


    for ( i = 1; i <= 100; i++ )
    {
        AVLTree_Insert(pTree, (void *)i, IntCompare);
    }


    for ( i = 1; i < 100; i++ )
    {
        pCurNode = BinTree_FindNode(pTree->pRoot, (void *)i, IntCompare);
        if ( pCurNode == NULL )
        {
            printf("TestCase_AVLTreeNode_GetNext(): cannot find nod %d\n", i);
        }

        pNode = AVLTreeNode_GetNext(pCurNode);

        assertTrue(pCurNode->pData == (void *)i && pNode->pData == (void *)(i+1));

    }

    pCurNode = BinTree_FindNode(pTree->pRoot, (void *)100, IntCompare);
    if ( pCurNode == NULL )
    {
        printf("TestCase_AVLTreeNode_GetNext(): cannot find nod %d\n", i);
    }

    pNode = AVLTreeNode_GetNext(pCurNode);

    assertTrue(pCurNode->pData == (void *)100 && pNode == NULL);

    return;
}


void TestCase_AVLTree_EnumNextAndDelete(void)
{
    int i, k;
    AVLTREE *pTree;
    AVLTREENODE *pCurNode;

    for ( i = 1; i <= 100; i++ )
    {
        pTree = AVLTree_Create();
        for ( k = 1; k <= 100; k++ )
        {
            AVLTree_Insert(pTree, (void *)k, IntCompare);
        }
        pCurNode = BinTree_FindNode(pTree->pRoot, (void *)i, IntCompare);
        if ( pCurNode == NULL )
        {
            printf("TestCase_AVLTreeNode_GetNext(): cannot find nod %d\n", i);
        }

        pTree->pCursor = pCurNode;

        AVLTree_Delete(pTree, (void *)i, IntCompare, NULL);

        if ( i < 99 )
        {
            int nData = (int)(pTree->pCursor->pData);
            if ( nData != (i+1) )
            {
                printf("Error Check AVLTree_EnumNextAndDelete().\n");
            }
        }
        else
        {
            void *pData = AVLTree_EnumNext(pTree);
            assertTrue(pTree->pCursor == NULL || pData == NULL);
        }

        AVLTree_Destroy(pTree, NULL);
    }

    return;
}
