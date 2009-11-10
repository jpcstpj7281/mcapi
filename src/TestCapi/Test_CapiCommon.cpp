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
#include "TestApi.h"
#include "CapiGlobal.h"
#include "CapiCommon.h"


void TestCase_GetCacheAlignedAddr(void);
void TestCase_AtomicCAS(void);

void Test_CapiCommon()
{
    TestCase_Add(TestCase_GetCacheAlignedAddr);
    TestCase_Add(TestCase_AtomicCAS);
}

REGISTER_TESTFUNC(Test_CapiCommon)


void TestCase_GetCacheAlignedAddr(void)
{
    void * pAddr = (void *)0x8012ff45;
    void *pRet = GetCacheAlignedAddr(pAddr);
    printf( "Cache Aligned Addr for %x = %x\n", (UINT)pAddr, (UINT)pRet);
    assertTrue(pRet == (void *)0x8012ff80);

    pAddr = (void *)0x8012ff40;
    pRet = GetCacheAlignedAddr(pAddr);
    printf( "Cache Aligned Addr for %x = %x\n", (UINT)pAddr, (UINT)pRet);
    assertTrue(pRet == (void *)0x8012ff40);


    pAddr = (void *)0x8012ff39;
    pRet = GetCacheAlignedAddr(pAddr);
    printf( "Cache Aligned Addr for %x = %x\n", (UINT)pAddr, (UINT)pRet);
    assertTrue(pRet == (void *)0x8012ff40);

    pAddr = (void *)0x8012ff79;
    pRet = GetCacheAlignedAddr(pAddr);
    printf( "Cache Aligned Addr for %x = %x\n", (UINT)pAddr, (UINT)pRet);
    assertTrue(pRet == (void *)0x8012ff80);

}

void TestCase_AtomicCAS(void)
{
    int ret;
    int val;
    int oldval;
    int newval;

    val = 100;
    oldval = val;
    
    newval = 200;

    ret = AtomicCAS(&val, newval, oldval);

    printf("AtomicCAS(), oldval = %d, val = %d, ret = %d\n", oldval, val, ret);
}
