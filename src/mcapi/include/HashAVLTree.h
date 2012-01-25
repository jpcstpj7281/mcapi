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
 *	HashAVLTree.h
 *
 *	DESCRIPTION
 *		Interface to a generic HashAVLTree type.
 *
 *	HISTORY
 *		08-08-2004	create by zhouweiming.
 *
 */

#ifndef __HASHAVLTREE_H__
#define __HASHAVLTREE_H__


#ifdef __cplusplus
extern "C" {
#endif


typedef struct HASHAVLTREE_st {
    AVLTREENODE **ppBucket;	/* 索引表指针 */
    UINT 	uBucketCount;	/* 索引表的大小. */
    UINT 	uNodeCount;	/* 表中的实际节点个数. */
    
    UINT        uCurBucketNo;   /* 当前要执行的Bucket序号 */
    AVLTREENODE *pCurEntry;      /* 当前bucket中的下一个要执行的节点条目 */ 
} HASHAVLTREE;


HASHAVLTREE *HashAVLTree_Create(UINT uBucketCount);
void HashAVLTree_Destroy(HASHAVLTREE *pHashAVLTree, DESTROYFUNC DestroyFunc);


INT HashAVLTree_Insert(HASHAVLTREE *pHashAVLTree, void *pData, HASHFUNC HashFunc,
                      COMPAREFUNC CompareFunc);

INT HashAVLTree_Delete(HASHAVLTREE *pHashAVLTree, void *pData, 
                      HASHFUNC HashFunc, 
                      COMPAREFUNC CompareFunc,
                      DESTROYFUNC DestroyFunc);

void * HashAVLTree_Find(HASHAVLTREE *pHashAVLTree, void *pData, 
                       HASHFUNC HashFunc, 
                       COMPAREFUNC CompareFunc );

/**	哈希AVL树的查找节点函数

	@param	HASHAVLTREE *pHashAVLTree - 哈希AVL树指针	
	@param	void *pData - 要查找的数据指针	
	@param	HASHFUNC HashFunc - 哈希函数	
	@param	COMPAREFUNC CompareFunc - 数据比较回调函数	
	@return	AVLTREENODE * - 失败返回NULL，成功返回找到的节点指针	
*/
AVLTREENODE * HashAVLTree_FindNode(HASHAVLTREE *pHashAVLTree, void *pData, 
                       HASHFUNC HashFunc, 
                       COMPAREFUNC CompareFunc );


void HashAVLTree_EnumBegin(HASHAVLTREE *pHashAVLTree);

void *HashAVLTree_EnumNext(HASHAVLTREE *pHashAVLTree);




#ifdef __cplusplus
}
#endif

#endif /* __HASHAVLTREE_H__ */
