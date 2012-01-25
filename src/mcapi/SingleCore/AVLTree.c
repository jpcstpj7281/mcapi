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
#include "CapiGlobal.h"
#include "BinTree.h"
#include "AVLTree.h"


/**	AVL���Ĵ�������

	@param	void -��	
	@return	AVLTREE * -�ɹ����ش�����AVL��ָ�룬ʧ�ܷ���NULL��	
*/
AVLTREE *AVLTree_Create(void)
{
    AVLTREE *pTree;

    pTree = (AVLTREE *)malloc(sizeof(AVLTREE));
    if ( pTree != NULL )
    {
        pTree->pRoot = NULL;
        pTree->uNodeCount = 0;
        pTree->pCursor = NULL;
    }
    return pTree;
}

/**	AVL�����ͷź���������ĳ��ָ���ڵ�Ϊ���ڵ��AVL�������е�ȫ���ڵ㶼�ͷŵ�
    ���ú��������ʽ�����ͷ�

	@param	AVLTREENODE *pTreeNode - Ҫ�ͷŵĸ��ڵ�	
	@param	DESTROYFUNC DestroyFunc - �����ͷŻص�����	
	@return	void - ��	
*/
void AVLTreeNode_Destroy(AVLTREENODE *pTreeNode, DESTROYFUNC DestroyFunc)
{
    if ( pTreeNode != NULL )
    {
        if ( pTreeNode->pLeft != NULL )
        {
            AVLTreeNode_Destroy(pTreeNode->pLeft, DestroyFunc);
        }
        if ( pTreeNode->pRight != NULL )
        {
            AVLTreeNode_Destroy(pTreeNode->pRight, DestroyFunc);
        }
        if ( DestroyFunc != NULL && pTreeNode->pData != NULL )
        {
            (*DestroyFunc)(pTreeNode->pData);
        }
        free(pTreeNode);
    }
}

/**	AVL�����ͷź�������һ��AVL���ͷŵ�

	@param	AVLTREE *pTree - Ҫ�ͷŵ�AVL��ָ��	
	@param	DESTROYFUNC DestroyFunc - �����ͷŻص�����	
	@return	void - ��	
*/
void AVLTree_Destroy(AVLTREE *pTree, DESTROYFUNC DestroyFunc)
{
    if ( pTree == NULL )
    {
        return;
    }
    AVLTreeNode_Destroy( pTree->pRoot, DestroyFunc );
    free( pTree );
}


/**	AVL���Ĳ��Һ����������˶����������Ĳ��Һ������в���

	@param	AVLTREE *pTree - Ҫ���ҵ�AVL��ָ��	
	@param	void *pData - Ҫ���ҵ�����ָ��	
	@param	COMPAREFUNC CompareFunc - ���ݱȽϺ���
	@return	void * - �ɹ����ز��ҵ���Ŀ�����ݣ�ʧ�ܷ���NULL	
*/
void *AVLTree_Find(AVLTREE *pTree, void *pData, COMPAREFUNC CompareFunc)
{
    return BinTree_Find(pTree->pRoot, pData, CompareFunc);
}





/**	AVL���Ļ�ȡ����������������֮�͵ĺ���
    ��ȡ�����ڵ�����������������ĸ������Ӳ����

    @param	AVLTREENODE *pStartNode - Ҫ�����Ľڵ�	
    @return	UINT - ����������������������֮��	
*/
UINT AVLTree_GetSubTreeCount(AVLTREENODE *pStartNode)
{
    UINT    uLeftCount;
    UINT    uRightCount;
    AVLTREENODE *pNode = pStartNode;

    if ( pNode->pLeft)
    {
        uLeftCount = pNode->pLeft->uCount;
    }
    else
    {
        uLeftCount = 0;
    }
    if ( pNode->pRight)
    {
        uRightCount = pNode->pRight->uCount;
    }
    else
    {
        uRightCount = 0;
    }

    return (uLeftCount + uRightCount);
}

/**	AVL������������

	@param	AVLTREE *pTree - AVL��ָ��	
	@param	AVLTREENODE *pStartNode - Ҫ�������������Ľڵ�ָ��	
	@return	void - ��	
*/
void AVLTree_RotateLeft( AVLTREE *pTree, AVLTREENODE *pStartNode )
{
    BinTree_RotateLeft((BINTREEBASENODE *)pStartNode, &(pTree->pRoot));
}

/**	AVL������������

	@param	AVLTREE *pTree - AVL��ָ��	
	@param	AVLTREENODE *pStartNode - Ҫ�������������Ľڵ�ָ��	
	@return	void - ��	
*/
void AVLTree_RotateRight(AVLTREE *pTree, AVLTREENODE *pStartNode )
{
    BinTree_RotateRight((BINTREEBASENODE *)pStartNode, &(pTree->pRoot));
}

/**	AVL����˫��ת����������������������

	@param	AVLTREENODE **ppRoot - AVL���ĸ��ڵ�ָ��	
	@param	AVLTREENODE *pStartNode - Ҫ��ת�Ľڵ�	
	@return	void - ��
*/
void AVLTree_RotateLeftRight(AVLTREENODE **ppRoot, AVLTREENODE *pStartNode)
{
    AVLTREENODE *pANode;
    AVLTREENODE *pBNode;
    AVLTREENODE *pCNode;

    pANode = pStartNode;
    pBNode = pANode->pLeft;
    pCNode = pBNode->pRight;

    BinTree_RotateLeft(pBNode, ppRoot);
    BinTree_RotateRight(pANode, ppRoot);

    switch ( pCNode->nMagic )
    {
    case 1:
        /* ������C�ڵ������������ */
        pANode->nMagic = -1;
        pBNode->nMagic = 0;
        break;
    case -1:
        /* ������C�ڵ������������ */
        pANode->nMagic = 0;
        pBNode->nMagic = 1;
        break;
    default:
        /* C�ڵ���ǲ�������� */
        pANode->nMagic = 0;
        pBNode->nMagic = 0;
        break;        
    }
    pCNode->nMagic = 0;
}

/**	AVL����˫��ת����

	@param	AVLTREENODE **ppRoot - AVL�����ڵ�ָ��
	@param	AVLTREENODE *pStartNode - Ҫ��ת�Ľڵ�ָ��	
	@return	void - ��
*/void AVLTree_RotateRightLeft(AVLTREENODE **ppRoot, AVLTREENODE *pStartNode)
{
    AVLTREENODE *pANode;
    AVLTREENODE *pBNode;
    AVLTREENODE *pCNode;

    pANode = pStartNode;
    pBNode = pANode->pRight;
    pCNode = pBNode->pLeft;

    BinTree_RotateRight(pBNode, ppRoot);
    BinTree_RotateLeft(pANode, ppRoot);

    switch ( pCNode->nMagic )
    {
    case 1:
        /* ������C�ڵ������������ */
        pANode->nMagic = 0;
        pBNode->nMagic = -1;
        break;
    case -1:
        /* ������C�ڵ������������ */
        pANode->nMagic = 1;
        pBNode->nMagic = 0;
        break;
    default:
        /* C�ڵ���ǲ�������� */
        pANode->nMagic = 0;
        pBNode->nMagic = 0;
        break;        
    }
    pCNode->nMagic = 0;
}

/**	AVL���ĵ���ƽ�����Ӻ�������Ҫ�ǵ����������ʱ��ƽ��

    @param	AVLTREENODE *pStartNode - Ҫ����ƽ�����ʼ�ڵ�	
    @param	void *pData - ���������ָ��	
    @param	COMPAREFUNC CompareFunc - ���ݱȽϻص�����	
    @return	void - ��	
*/
static void AVLTree_FixBalance(AVLTREENODE *pStartNode, 
                               void *pData, COMPAREFUNC CompareFunc)
{
    AVLTREENODE *pNode;
    AVLTREENODE *pSearchNode;
    INT nResult; 

    pNode = pStartNode;
    while ( pNode != NULL )
    {
        pSearchNode = pNode;

        nResult = (*CompareFunc)(pNode->pData, pData);
        if ( nResult < 0 )
        {
            pNode = pNode->pRight;
            pSearchNode->nMagic -= 1;
        }
        else if ( nResult > 0 )
        {
            pNode = pNode->pLeft;
            pSearchNode->nMagic += 1;
        }
        else
        {
            /* �ҵ���ͬ�ؼ��ʵĽڵ�, ��ֹ */
            break;
        }
    }
    return;
}

/**	AVL����������еĵ����������Ӻ���
    ���Ὣ���ڵ㵽����ڵ�·�������нڵ㣨����������ڵ㣩��
    �������Ӽ�1

    @param	AVLTREENODE *pInsertNode - ����ڵ�	
    @return	void - ��	
*/
static void AVLTree_FixCountForInsert(AVLTREENODE *pInsertNode)
{
    AVLTREENODE *pNode = pInsertNode;

    pNode = pNode->pParent;
    while ( pNode != NULL )
    {
        pNode->uCount += 1;
        pNode = pNode->pParent;
    }
}


/**	AVL���Ĳ����������

	@param	AVLTREE *pTree - AVL��	
	@param	void *pData - Ҫ���������ָ��	
	@param	COMPAREFUNC CompareFunc - ���ݱȽϺ���	
	@return	INT - CAPI_SUCCESS��ʾ�ɹ���ʧ�ܷ���CAPI_FAILED	
*/
INT AVLTree_Insert(AVLTREE *pTree, void *pData, COMPAREFUNC CompareFunc)
{
    INT nRet;
    if ( pTree == NULL || pData == NULL || CompareFunc == NULL )
    {
        return CAPI_FAILED;
    }
    nRet = AVLTreeNode_Insert(&(pTree->pRoot), pData, CompareFunc); 
    if ( nRet != CAPI_FAILED )
    {
        pTree->uNodeCount += 1;
    }
    return nRet;
}

/**	AVL���Ĳ������ݺ�������һ�����ݲ��뵽AVL����

	@param	AVLTREENODE **ppRootNode - Ҫ�����AVL�����ڵ�ָ��
	@param	void *pData - Ҫ���������	
	@param	COMPAREFUNC CompareFunc - ���ݱȽϻص�����	
	@return	INT - CAPI_SUCCESS��ʾ����ɹ���CAPI_FAILED��ʾʧ�ܣ�ʧ�ܵ�ԭ����
                  �ڴ����ʧ�ܺ��Ѵ�����ͬ�ؼ��������������
*/
INT AVLTreeNode_Insert(AVLTREENODE **ppRootNode, void *pData, 
                       COMPAREFUNC CompareFunc)
{
    AVLTREENODE *pNode;
    AVLTREENODE *pANode;
    AVLTREENODE *pNewNode;
    AVLTREENODE *pSearchNode;
    AVLTREENODE *pBNode;
    AVLTREENODE *pCNode;
    INT         nRet;

    nRet = 0;
    pANode = NULL;
    pSearchNode = NULL;

    /* ����Ҫ����Ľڵ�λ�ã������ڲ��ҹ�����Ҫ��¼���һ����ƽ��Ľڵ� */
    pNode = *ppRootNode;
    while ( pNode != NULL )
    {
        pSearchNode = pNode;
        if ( pSearchNode->nMagic == -1 || pSearchNode->nMagic == 1 )
        {
            pANode = pSearchNode;
        }
        nRet = (*CompareFunc)(pNode->pData, pData);
        if ( nRet < 0 )
        {
            pNode = pNode->pRight;
        }
        else if ( nRet > 0 )
        {
            pNode = pNode->pLeft;
        }
        else
        {
            /* �ҵ���ͬ�ؼ��ʵĽڵ㣬����ʧ�� */
            return CAPI_FAILED;
        }
    }

    pNewNode = (AVLTREENODE *)malloc( sizeof(AVLTREENODE) );
    if ( pNewNode == NULL )
    {
        return CAPI_FAILED;
    }
    pNewNode->pData = pData;
    pNewNode->pLeft = NULL;
    pNewNode->pRight = NULL;
    pNewNode->pParent = pSearchNode;
    pNewNode->nMagic = 0;
    pNewNode->uCount = 1;

    if ( pSearchNode == NULL )
    {
        *ppRootNode = pNewNode;
        return CAPI_SUCCESS;
    }

    if ( nRet < 0 )
    {
        pSearchNode->pRight = pNewNode;
    }
    else
    {
        pSearchNode->pLeft = pNewNode;
    }

    /* �޸��²���ڵ㵽���ڵ�·���ϵ����нڵ��Ԫ�ظ��� */
    AVLTree_FixCountForInsert(pNewNode);

    /* �����ڲ�ƽ��Ľڵ㣬ֱ�Ӳ�������޸�ƽ�����Ӽ��� */
    if ( pANode == NULL )
    {
        /* �޸ĴӸ��ڵ㵽����ڵ��ƽ������ */
        AVLTree_FixBalance(*ppRootNode, pData, CompareFunc);
       
        return CAPI_SUCCESS;
    }
    
    nRet = (*CompareFunc)(pANode->pData, pData);

    /* ���´�����ڲ�ƽ��ڵ����� */
    if ( ( pANode->nMagic == 1 && nRet < 0 )
        || (pANode->nMagic == -1 && nRet > 0 ) )
    {
        /* A�ڵ�ƽ������Ϊ1�Ҳ���ڵ�����������������
         * �Լ�A�ڵ�ƽ������Ϊ-1�Ҳ����������������
         * ����������������ƽ������ֻ���޸���ؽڵ�ƽ������
         */
        AVLTree_FixBalance(pANode, pData, CompareFunc);
    }
    else if ( pANode->nMagic == 1 && nRet > 0 )
    {
        if ( (*CompareFunc)(pANode->pLeft->pData, pData) > 0 )
        {
            /* LL�Ͳ�ƽ��, ������A�ڵ������������������ */
            pBNode = pANode->pLeft;
            BinTree_RotateRight(pANode, ppRootNode);
            AVLTree_FixBalance(pBNode, pData, CompareFunc);
            pANode->nMagic = 0;
            pBNode->nMagic = 0;

            pBNode->uCount = pANode->uCount;
            pANode->uCount = AVLTree_GetSubTreeCount(pANode) + 1;
        }
        else
        {
            INT nRetVal;

            /* LR�Ͳ�ƽ��, ������A�ڵ������������������ */
            pBNode = pANode->pLeft;
            pCNode = pBNode->pRight;
            nRetVal = (*CompareFunc)(pCNode->pData, pData) ;
            if ( nRetVal > 0 )
            { 
                /* �½ڵ���� C �ڵ���������е���� */
                pCNode->nMagic += 1;
            }
            else if ( nRetVal < 0 )
            {
                /* �½ڵ���� C �ڵ���������е���� */
                pCNode->nMagic -= 1;
            }

            /* ˫��ת������A��B��C �ڵ�� nMagic ֵ�Ѿ������ȷ */
            AVLTree_RotateLeftRight(ppRootNode, pANode);

            /* Fixed by fanxueqiang on 2011-11-28 */
            /* NOTE: AVLTree_FixBalance() ��Ӱ����ڵ㣬 �����ڵ��Ѿ���
             * AVLTree_RotateRightLeft() ��������д��ȷ���������޸� */
            if ( nRetVal > 0 )
            {
                /* �½ڵ���� C �ڵ���������е���� */
                /* AVLTree_FixBalance() �Ĳ���ֻ���ڲ�����ʹ�������ĸ߶� + 1
                 * �������Ч������һ�������������룩����������������д���
                 * ����������������һ��ʹ�� B �ڵ�Ϊ������ʱ�ĸ߶� + 1��
                 * ֻ��ȷ�� B �ڵ���������ĸ߶� + 1��
                 * ���Դ� B �ڵ����������ʼ */
                /*AVLTree_FixBalance(pBNode, pData, CompareFunc);*/
                AVLTree_FixBalance(pBNode->pRight, pData, CompareFunc);
            }
            else if ( nRetVal < 0 )
            {
                /* �½ڵ���� C �ڵ���������е���� */
                /*AVLTree_FixBalance(pANode, pData, CompareFunc);*/
                AVLTree_FixBalance(pANode->pLeft, pData, CompareFunc);
            }
            else
            {
            }

            pCNode->uCount = pANode->uCount;
            pBNode->uCount = AVLTree_GetSubTreeCount(pBNode) + 1;
            pANode->uCount = AVLTree_GetSubTreeCount(pANode) + 1;
        }
    }
    else /* pANode->nMagic == -1 && nRet < 0 �����*/
    {
        if ( (*CompareFunc)(pANode->pRight->pData, pData) > 0 )
        {
            INT nRetVal;

            /* RL�Ͳ�ƽ��, ������A�ڵ������������������ */
            pBNode = pANode->pRight;
            pCNode = pBNode->pLeft;
            nRetVal = (*CompareFunc)(pCNode->pData, pData);
            if ( nRetVal > 0 )
            { 
                pCNode->nMagic += 1;
            }
            else if ( nRetVal < 0 )
            {
                pCNode->nMagic -= 1;
            }

            AVLTree_RotateRightLeft(ppRootNode, pANode);

            if ( nRetVal > 0 )
            {
                /*AVLTree_FixBalance(pANode, pData, CompareFunc);*/
                AVLTree_FixBalance(pANode->pRight, pData, CompareFunc);
            }
            else if ( nRetVal < 0 )
            {
                /*AVLTree_FixBalance(pBNode, pData, CompareFunc);*/
                AVLTree_FixBalance(pBNode->pLeft, pData, CompareFunc);
            }
            else
            {
            }

            pCNode->uCount = pANode->uCount;
            pBNode->uCount = AVLTree_GetSubTreeCount(pBNode) + 1;
            pANode->uCount = AVLTree_GetSubTreeCount(pANode) + 1;
        }
        else
        {
            /* RR�Ͳ�ƽ��, ������A�ڵ������������������ */
            pBNode = pANode->pRight;
            BinTree_RotateLeft(pANode, ppRootNode);
            AVLTree_FixBalance(pBNode, pData, CompareFunc);
            pANode->nMagic = 0;
            pBNode->nMagic = 0;
            pBNode->uCount = pANode->uCount;
            pANode->uCount = AVLTree_GetSubTreeCount(pANode) + 1;

        }
    }
    return CAPI_SUCCESS;
}

/**	AVL��ɾ�������еĵ����������Ӻ���
    ���Ὣ���ڵ㵽A�ڵ�·�������нڵ㣨����A�ڵ㣩��
    �������Ӽ�1

    @param	AVLTREENODE *pANode - ɾ�������е�A�ڵ�	
    @return	void - ��	
*/
static void AVLTree_FixCountForDelete(AVLTREENODE *pANode)
{
    AVLTREENODE *pNode = pANode;

    while ( pNode != NULL )
    {
        pNode->uCount -= 1;
        pNode = pNode->pParent;
    }
}

/**	AVL����ɾ����������ƽ�⺯������ɾ����������

	@param	AVLTREENODE **ppRoot - ָ��AVL���ĸ��ڵ�ָ���ָ��	
	@param	AVLTREENODE *pNode - Ҫ����ƽ��Ľڵ�	
	@param	void *pData - ɾ��������ָ��	
	@param	COMPAREFUNC CompareFunc - ���ݱȽϻص�����	
	@return	void - ��	
*/
void AVLTree_AdjustBalanceForDelete(AVLTREENODE **ppRoot, AVLTREENODE *pNode, 
                                    void *pData, COMPAREFUNC CompareFunc )
{
    AVLTREENODE     *pANode;
    AVLTREENODE     *pParentNode;
    AVLTREENODE     *pBNode;
    AVLTREENODE     *pCNode;

    pANode = pNode;
    while ( pANode != NULL )
    {
        switch( pANode->nMagic )
        {
        case 0:
            if ( pANode == *ppRoot )
            {
                /* pANodeΪ���ڵ㣬�߶ȼ�����1�������������߶���ȣ�������� */
                break;
            }
            else 
            {
                pParentNode = pANode->pParent;
                if ( pANode == pParentNode->pLeft )
                {
                    pParentNode->nMagic -= 1;
                }
                else
                {
                    pParentNode->nMagic += 1;
                }
                /* ��pANode ָ�����ĸ��ڵ㣬�����������ĸ��ڵ�Ĳ�ƽ����� */
                pANode = pParentNode;
                continue;
            }
        case -1: 
        case 1:
            /* pANodeԭ����ƽ������Ϊ0��ɾ�����������󣬸߶�δ�ı䣬��˲���Ҫ
             * �ٵ������˳����� 
             */
            break;
        case -2: /* L�Ͳ�ƽ����� */
            pBNode = pANode->pRight;
            if ( pBNode->nMagic == 0 )
            {
                /* L0�Ͳ�ƽ����� */
                BinTree_RotateLeft(pANode, ppRoot);
                pANode->nMagic = -1;
                pBNode->nMagic = 1;

                pBNode->uCount = pANode->uCount;
                pANode->uCount = AVLTree_GetSubTreeCount(pANode)+1;
                break;
            }
            else if ( pBNode->nMagic == -1 )
            {
                /* L-1�Ͳ�ƽ����� */
                pParentNode = pANode->pParent;
                if ( pParentNode != NULL )
                {
                    if ( pANode == pParentNode->pLeft )
                    {
                        pParentNode->nMagic -= 1;
                    }
                    else
                    {
                        pParentNode->nMagic += 1;
                    }
                }
                
                BinTree_RotateLeft(pANode, ppRoot);
                pANode->nMagic = 0;
                pBNode->nMagic = 0;
                pBNode->uCount = pANode->uCount;
                pANode->uCount = AVLTree_GetSubTreeCount(pANode)+1;

                /* ��pANode ָ�����ĸ��ڵ㣬�����������ĸ��ڵ�Ĳ�ƽ����� */
                pANode = pParentNode;
            }
            else /* pBNode->nMagic == 1����� */
            {
                /* L1�͵���� */
                pParentNode = pANode->pParent;
                if ( pParentNode != NULL )
                {
                    if ( pANode == pParentNode->pLeft )
                    {
                        pParentNode->nMagic -= 1;
                    }
                    else
                    {
                        pParentNode->nMagic += 1;
                    }
                }
                pBNode = pANode->pRight;
                pCNode = pBNode->pLeft;

                AVLTree_RotateRightLeft(ppRoot, pANode);
                
                pCNode->uCount = pANode->uCount;
                pANode->uCount = AVLTree_GetSubTreeCount(pANode)+1;
                pBNode->uCount = AVLTree_GetSubTreeCount(pBNode)+1;

                /* ��pANode ָ�����ĸ��ڵ㣬�����������ĸ��ڵ�Ĳ�ƽ����� */
                pANode = pParentNode;

            }
            continue; /* ����while() ѭ�� */
        case 2: /* R�Ͳ�ƽ����� */
            pBNode = pANode->pLeft;
            if ( pBNode->nMagic == 0 )
            {
                /* R0�Ͳ�ƽ����� */
                BinTree_RotateRight(pANode, ppRoot);
                pANode->nMagic = 1;
                pBNode->nMagic = -1;
                pBNode->uCount = pANode->uCount;
                pANode->uCount = AVLTree_GetSubTreeCount(pANode)+1;
                break;
            }
            else if ( pBNode->nMagic == -1 )
            {
                /* R-1�Ͳ�ƽ����� */
                pParentNode = pANode->pParent;
                if ( pParentNode != NULL )
                {
                    if ( pANode == pParentNode->pLeft )
                    {
                        pParentNode->nMagic -= 1;
                    }
                    else
                    {
                        pParentNode->nMagic += 1;
                    }
                }
                pBNode = pANode->pLeft;
                pCNode = pBNode->pRight;
                AVLTree_RotateLeftRight(ppRoot, pANode);
                pCNode->uCount = pANode->uCount;
                pANode->uCount = AVLTree_GetSubTreeCount(pANode)+1;
                pBNode->uCount = AVLTree_GetSubTreeCount(pBNode)+1;
                /* ��pANode ָ�����ĸ��ڵ㣬�����������ĸ��ڵ�Ĳ�ƽ����� */
                pANode = pParentNode;
            }
            else /* pBNode->nMagic == 1����� */
            {
                /* R1�͵���� */
                pParentNode = pANode->pParent;
                if ( pParentNode != NULL )
                {
                    if ( pANode == pParentNode->pLeft )
                    {
                        pParentNode->nMagic -= 1;
                    }
                    else
                    {
                        pParentNode->nMagic += 1;
                    }
                }
                BinTree_RotateRight(pANode, ppRoot);
                pANode->nMagic = 0;
                pBNode->nMagic = 0;
                pBNode->uCount = pANode->uCount;
                pANode->uCount = AVLTree_GetSubTreeCount(pANode)+1;
                /* ��pANode ָ�����ĸ��ڵ㣬�����������ĸ��ڵ�Ĳ�ƽ����� */
                pANode = pParentNode;
            }
            continue; /* ����while() ѭ�� */
        default:
            break;
        } /* switch ( pANode->nMagic ) */
        break;
    }
}



/**	AVL����ɾ����������

	@param	AVLTREE *pTree - AVL��ָ��	
	@param	void *pData - Ҫɾ��������ָ��	
	@param	COMPAREFUNC CompareFunc - ���ݱȽϻص�����	
	@param	DESTROYFUNC DestroyFunc - �����ͷŻص�����	
	@return	INT - CAPI_FAILED��ʾʧ�ܣ�CAPI_SUCCESS��ʾ�ɹ�	
*/
INT AVLTree_Delete(AVLTREE *pTree, void *pData, COMPAREFUNC CompareFunc,
                   DESTROYFUNC DestroyFunc)
{
    AVLTREENODE *pCursor;
    void *pPopData;

    if ( pTree->pRoot == NULL || pData == NULL 
        || CompareFunc == NULL )
    {
        return CAPI_FAILED;
    }
    
    pCursor = pTree->pCursor;

    pPopData = AVLTreeNode_Pop(&(pTree->pRoot), &pCursor, pData, CompareFunc);
    if ( pPopData != NULL )
    {
        pTree->pCursor = pCursor;

        if ( pCursor != NULL )
        {
            if ( (*CompareFunc)(pCursor->pData, pData) < 0 )
            {
                AVLTree_EnumNext(pTree);
            }
        }

        if ( DestroyFunc != NULL )
        {
            (*DestroyFunc)(pPopData);
        }

        pTree->uNodeCount -= 1;

        return CAPI_SUCCESS;
    }
    else
    {
        return CAPI_NOT_FOUND;
    }
}


/**	AVL����ɾ���ڵ㺯��

	@param	AVLTREENODE **ppRoot - ָ��AVL�����ڵ�ָ���ָ��	
	@param	void *pData - Ҫɾ��������	
	@param	COMPAREFUNC CompareFunc - ���ݱȽϻص�����	
	@param	DESTROYFUNC DestroyFunc - �����ͷŻص�����	
	@return	INT - CAPI_FAILED��ʾʧ�ܣ�CAPI_SUCCESS��ʾ�ɹ�		
*/
INT AVLTreeNode_Delete(AVLTREENODE **ppRoot, AVLTREENODE **ppCursor, void *pData, 
                       COMPAREFUNC CompareFunc, DESTROYFUNC DestroyFunc)
{

    AVLTREENODE  *pNode;
    AVLTREENODE  *pANode;
    AVLTREENODE  *pDelNode;
    void         *pDelData;
    INT         nRet = 0;

    pNode = *ppRoot;

    while ( pNode != NULL )
    {
        nRet = (*CompareFunc)(pNode->pData, pData);
        if ( nRet < 0 )
        {
            pNode = pNode->pRight;
        }
        else if ( nRet > 0 )
        {
            pNode = pNode->pLeft;
        }
        else
        {
            break;
        }
    }

    if ( pNode == NULL )
    {
        return CAPI_NOT_FOUND;
    }

    pDelData = pNode->pData;
    if ( pNode->pLeft != NULL && pNode->pRight != NULL )
    {
        /* ������ҵ���pNode�������ӽڵ����� */
        pDelNode = pNode->pLeft;   
        while (pDelNode->pRight != 0)
        {
            pDelNode = pDelNode->pRight;
        }
        pANode = pDelNode->pParent;  

        pNode->pData = pDelNode->pData;
        
		if ( pDelNode == pANode->pLeft )
        {
            pANode->nMagic -= 1;
        }
        else
        {
            pANode->nMagic += 1;
        }
        
		if (pDelNode != pNode->pLeft) 
        {
            pANode->pRight = pDelNode->pLeft;
        }
        else
        {
            pANode->pLeft = pDelNode->pLeft;
        }

        if ( pDelNode->pLeft != NULL )
        {
            pDelNode->pLeft->pParent = pANode;
        }
    }
    else
    {
        pANode = pNode;
        /* �������ֻ��һ���ӽڵ����� */
        if ( pNode->pLeft != NULL )
        {
            /* ֻ����ڵ����� */
            pDelNode = pNode->pLeft;
            pNode->pData = pDelNode->pData;
            pNode->pLeft = NULL;
            pANode->nMagic -= 1;
        }
        else if ( pNode->pRight != NULL )
        {
            /* ֻ���ҽڵ����� */
            pDelNode = pNode->pRight;
            pNode->pData = pDelNode->pData;
            pNode->pRight = NULL;
            pANode->nMagic += 1;
        }
        else
        {
            /* ����ɾ���ڵ�������ӽڵ㶼�����ڵ���� */
            pANode = pNode->pParent;
            pDelNode = pNode;
            if ( pANode == NULL )
            {
                *ppRoot = pANode;
            }
            else if ( pANode->pLeft == pNode )
            {
                pANode->pLeft = NULL;
                pANode->nMagic -= 1;
            }
            else
            {
                pANode->pRight = NULL;
                pANode->nMagic += 1;
            }
        }
    }
   
    /* ɾ����Ӧ�ڵ� */
    if ( pDelNode != NULL )
    {
        if ( pDelNode == *ppCursor )
        {
            AVLTREENODE *pNextNode = AVLTreeNode_GetNext(pDelNode);

            *ppCursor = pNextNode;
        }
        if ( DestroyFunc != NULL )
        {
            (*DestroyFunc)(pDelData);
        }
        free(pDelNode);
    }

    AVLTree_FixCountForDelete(pANode);

    /* ����ƽ�� */
    if ( pANode != NULL )
    {
        AVLTree_AdjustBalanceForDelete(ppRoot, pANode, pData, CompareFunc);
    }
    
    return CAPI_SUCCESS;
}



/**	AVL����ɾ���ڵ㺯��

	@param	AVLTREENODE **ppRoot - ָ��AVL�����ڵ�ָ���ָ��	
	@param	void *pData - Ҫɾ��������	
	@param	COMPAREFUNC CompareFunc - ���ݱȽϻص�����	
	@return	void * - NULL��ʾʧ�ܣ��ǿձ�ʾ�����Ķ�Ӧ�ڵ�����		
*/
void * AVLTreeNode_Pop(AVLTREENODE **ppRoot, AVLTREENODE **ppCursor, void *pData, 
                       COMPAREFUNC CompareFunc)
{

    AVLTREENODE  *pNode;
    AVLTREENODE  *pANode;
    AVLTREENODE  *pDelNode;
    void         *pDelData;
    INT         nRet = 0;

    pNode = *ppRoot;

    while ( pNode != NULL )
    {
        nRet = (*CompareFunc)(pNode->pData, pData);
        if ( nRet < 0 )
        {
            pNode = pNode->pRight;
        }
        else if ( nRet > 0 )
        {
            pNode = pNode->pLeft;
        }
        else
        {
            break;
        }
    }

    if ( pNode == NULL )
    {
        return NULL;
    }

    pDelData = pNode->pData;
    if ( pNode->pLeft != NULL && pNode->pRight != NULL )
    {
        /* ������ҵ���pNode�������ӽڵ����� */
        pDelNode = pNode->pLeft;   
        while (pDelNode->pRight != 0)
        {
            pDelNode = pDelNode->pRight;
        }
        pANode = pDelNode->pParent;  

        pNode->pData = pDelNode->pData;
        
		if ( pDelNode == pANode->pLeft )
        {
            pANode->nMagic -= 1;
        }
        else
        {
            pANode->nMagic += 1;
        }
        
		if (pDelNode != pNode->pLeft) 
        {
            pANode->pRight = pDelNode->pLeft;
        }
        else
        {
            pANode->pLeft = pDelNode->pLeft;
        }

        if ( pDelNode->pLeft != NULL )
        {
            pDelNode->pLeft->pParent = pANode;
        }
    }
    else
    {
        pANode = pNode;
        /* �������ֻ��һ���ӽڵ����� */
        if ( pNode->pLeft != NULL )
        {
            /* ֻ����ڵ����� */
            pDelNode = pNode->pLeft;
            pNode->pData = pDelNode->pData;
            pNode->pLeft = NULL;
            pANode->nMagic -= 1;
        }
        else if ( pNode->pRight != NULL )
        {
            /* ֻ���ҽڵ����� */
            pDelNode = pNode->pRight;
            pNode->pData = pDelNode->pData;
            pNode->pRight = NULL;
            pANode->nMagic += 1;
        }
        else
        {
            /* ����ɾ���ڵ�������ӽڵ㶼�����ڵ���� */
            pANode = pNode->pParent;
            pDelNode = pNode;
            if ( pANode == NULL )
            {
                *ppRoot = pANode;
            }
            else if ( pANode->pLeft == pNode )
            {
                pANode->pLeft = NULL;
                pANode->nMagic -= 1;
            }
            else
            {
                pANode->pRight = NULL;
                pANode->nMagic += 1;
            }
        }
    }
   
    /* ɾ����Ӧ�ڵ� */
    if ( pDelNode != NULL )
    {
        if ( pDelNode == *ppCursor )
        {
            AVLTREENODE *pNextNode = AVLTreeNode_GetNext(pDelNode);

            *ppCursor = pNextNode;
        }
        free(pDelNode);
    }

    AVLTree_FixCountForDelete(pANode);

    /* ����ƽ�� */
    if ( pANode != NULL )
    {
        AVLTree_AdjustBalanceForDelete(ppRoot, pANode, pData, CompareFunc);
    }
    
    return pDelData;
}




static int AVLTree_CheckMagic_Rec(AVLTREENODE *pNode, VISITFUNC VisitFunc)
{
    int nLeftDeep, nRightDeep;
    int nDepth = 0;

    if (pNode == NULL)
    {
        return 0;
    }

    nLeftDeep = AVLTree_CheckMagic_Rec(pNode->pLeft, VisitFunc);
    nRightDeep = AVLTree_CheckMagic_Rec(pNode->pRight, VisitFunc);

    if ( nLeftDeep == -1 || nRightDeep == -1 )
    {
        return -1;
    }

    if (nLeftDeep > nRightDeep)
    {
        nDepth = nLeftDeep + 1;
    }
    else
    {
        nDepth = nRightDeep + 1;
    }

    if ( pNode->nMagic == nLeftDeep - nRightDeep )
    {
        return nDepth;
    }
    else
    {
        if ( VisitFunc != NULL )
        {
            (*VisitFunc)(pNode->pData);
        }
        return -1;
    }

    return nDepth;
}

int AVLTree_CheckMagic(AVLTREE *pTree, VISITFUNC VisitFunc)
{
    int nRet = AVLTree_CheckMagic_Rec(pTree->pRoot, VisitFunc);
    return nRet != -1;
}


#if 0
AVLTree_Delete()
{
    ���Ҵ�ɾ���ڵ�;
    //���ݴ�ɾ���ڵ�������ӽڵ��Ƿ���������ȷ��A�ڵ�;
    if ( ��ɾ���ڵ�������ӽڵ�������� )
    {
        A�ڵ� = ��ɾ���ڵ�ĸ��ڵ�;
        ʵ��ɾ���Ľڵ� = ��ɾ���ڵ�;
        ����A�ڵ��ƽ������;
    }
    else if ( ��ɾ���ڵ�������ӽڵ������)
    {
        ���Ҵ�ɾ���ڵ��������е����ڵ�
        if ( ��ɾ���ڵ������������ڵ� == ��ɾ���ڵ����ڵ� )
        {
            ����ɾ���ڵ�������Ƴ�;
            A�ڵ� �� ��ڵ�;
            ʵ��ɾ���ڵ� = ��ɾ���ڵ�;
        }
        else
        {
            �������������ڵ�����ݺʹ�ɾ���ڵ�����ݽ��н���;
            A�ڵ� = ��ɾ���ڵ������������ڵ�ĸ��ڵ�;
            ʵ��ɾ���ڵ� = ���������ڵ�;
        }
        ����A�ڵ��ƽ������;
    }
    else // ��ɾ���ڵ�ֻ��һ���ӽڵ���ڵ����
    {
        ʹ�ô��ڵ��ӽڵ��滻��ɾ���ڵ�;
        A�ڵ� = ȡ����ɾ���ڵ���ӽڵ�;
        ʵ��ɾ���ڵ� = ��ɾ���ڵ�;
    }

    �ͷ�ʵ��ɾ���ڵ���ڴ�;

    while ( A�ڵ� != NULL )
    {
        switch ( A�ڵ��ƽ������ )
        {
        case -1:
        case 1:
            // A�ڵ�ԭ����ƽ������Ϊ0��ɾ�����������󣬸߶�δ�ı䣬��˲���Ҫ
            // �ٵ������˳����� 
            break;
        case 0:
            if ( A�ڵ� == ���ڵ� ) 
            {
                // pANodeΪ���ڵ㣬�߶ȼ�����1�������������߶���ȣ��������
                break;
            }
            else 
            {
                �޸�A�ڵ�ĸ��ڵ��ƽ������;
                A�ڵ� �� A�ڵ�ĸ��ڵ�;
                continue;  //����whileѭ��
            }
        case -2: // L�Ͳ�ƽ����� 
            B�ڵ� = A�ڵ���ҽڵ�;
            if ( B�ڵ�ƽ������ == 0 )
            {
                // L0�Ͳ�ƽ����� 
                ��A�ڵ������������;
                A�ڵ�ƽ������ = -1;
                B�ڵ�ƽ������ = 1;
                break;
            }
            else if ( B�ڵ�ƽ������ == -1 )
            {
                // L-1�Ͳ�ƽ����� 
                ����A�ڵ�ĸ��ڵ��ƽ������;
                ��A�ڵ������������;
                A�ڵ�ƽ������ = 0;
                B�ڵ�ƽ������ = 0;

                A�ڵ� �� A�ڵ�ĸ��ڵ�; //����whileѭ��
            }
            else /* B�ڵ�ƽ������ == 1����� */
            {
                /* L1�͵���� */
                ����A�ڵ�ĸ��ڵ��ƽ������;
                ��B�ڵ��������;
                ��A�ڵ��������;

                A�ڵ� �� A�ڵ�ĸ��ڵ�; //����whileѭ��
            }
            continue; /* ����while() ѭ�� */
        case 2: // R�Ͳ�ƽ�����
            //��L�Ͳ�ƽ������ǶԳƵģ��˴���ȥ...
            continue;
        default:
            break;
        } // switch
    } //while
}
#endif

/**	AVL����ö�ٿ�ʼ����

	@param	AVLTREE *pTree - AVL��ָ��	
	@return	void - ��	
*/
void AVLTree_EnumBegin(AVLTREE *pTree)
{
    BINTREEBASENODE *pCursor;

    pCursor = pTree->pRoot;
    if ( pCursor != NULL )
    {
        while ( pCursor->pLeft != NULL )
        {
            pCursor = pCursor->pLeft;
        }
    }
    pTree->pCursor = pCursor;
}


/**	AVL����ö����һ��Ԫ�غ���
    �����������˳�����λ�ȡ��һ��Ԫ��

	@param	AVLTREE *pTree - AVL��ָ��	
	@return	void * - ���ػ�ȡ����һ��Ԫ�أ������һ��Ԫ�ز������򷵻ؿ�	
*/
void * AVLTree_EnumNext(AVLTREE *pTree)
{
    void *pData;
    AVLTREENODE *pCursor;

    if ( pTree->pCursor == NULL )
    {
        return NULL;
    }

    pCursor = pTree->pCursor;
    pData = pCursor->pData;

    if ( pCursor->pRight != NULL )
    {
        pCursor = pCursor->pRight;
        while ( pCursor->pLeft != NULL )
        {
            pCursor = pCursor->pLeft;
        }
    }
    else
    {
        AVLTREENODE *pParent = pCursor->pParent;
        while ( pParent != NULL && pParent->pRight == pCursor )
        {
            pCursor = pParent;
            pParent = pParent->pParent;
        }
        pCursor = pParent;
    }
    pTree->pCursor = pCursor;

    return pData;
}


AVLTREENODE *AVLTreeNode_GetNext(AVLTREENODE *pCurNode)
{
    AVLTREENODE *pCursor;

    if ( pCurNode == NULL )
    {
        return NULL;
    }

    pCursor = pCurNode;

    if ( pCursor->pRight != NULL )
    {
        pCursor = pCursor->pRight;
        while ( pCursor->pLeft != NULL )
        {
            pCursor = pCursor->pLeft;
        }
    }
    else
    {
        AVLTREENODE *pParent = pCursor->pParent;
        while ( pParent != NULL && pParent->pRight == pCursor )
        {
            pCursor = pParent;
            pParent = pParent->pParent;
        }
        pCursor = pParent;
    }

    return pCursor;
}

