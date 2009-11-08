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
#if _WIN32 || _WIN64
#include <windows.h>
#else
#include <sys/types.h>
#endif

#include "CapiGlobal.h"

#if _WIN32 

extern "C" BOOL AtomicCAS(LONG volatile *dest, LONG newvalue, LONG oldvalue)
{
    LONG 	ret;
    ret = InterlockedCompareExchange(dest, newvalue, oldvalue);
    if ( ret == oldvalue )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

extern "C" BOOL AtomicCAS64(LONGLONG volatile *dest, LONGLONG newvalue, LONGLONG oldvalue)
{
    // not implement yet
    return FALSE;
}
#if 0
extern "C" BOOL AtomicCAS64(LONGLONG volatile *dest, LONGLONG newvalue, LONGLONG oldvalue)
{
    LONGLONG 	ret;
    ret = InterlockedCompareExchange64(dest, newvalue, oldvalue);
    if ( ret == oldvalue )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

#else // For Linux 

inline int AtomicCAS(volatile void *ptr, int value, int comparand ) 
{                                                                                   
    int result;                                                                       

    __asm__ __volatile__("lock\ncmpxchg" "l" " %2,%1"                                     
        : "=a"(result), "=m"(*(int *)ptr)                                           
        : "q"(value), "0"(comparand)               
        : "memory");                                             
    return result;                                                                
}    

int64_t AtomicCAS64(volatile void *ptr, int64_t value, int64_t comparand )
{
    int64_t result;
    union {
        int64_t comparand_local;
        int32_t comparand_parts[2];
    };
    comparand_local = comparand;
    // EBX register saved for compliancy with position-independent code (PIC) rules on IA32
    __asm__ __volatile__ (
        "pushl %%ebx\n\t"
        "movl  (%%ecx),%%ebx\n\t"
        "movl  4(%%ecx),%%ecx\n\t"
        "lock\ncmpxchg8b (%2)\n\t"
        "popl  %%ebx"
        : "=A"(result), "=m"(*(int64_t *)ptr)
        : "S"(ptr),
        "a"(comparand_parts[0]),
        "d"(comparand_parts[1]),
        "c"(&value)
        : "memory", "esp");
    return result;
}

#endif


#ifndef _WIN32
LONG AtomicWrite(LONG volatile *Target, LONG Value)
{
    LONG	Old;

    do {
        Old = *Target;

    }while (!AtomicCAS(Target, Value, Old));

    return Old;
}

LONG AtomicIncrement(LONG volatile *Target)
{
    LONG	Old;

    do {
        Old = *Target;

    }while (!AtomicCAS(Target, Old + 1, Old));

    return Old;
}


LONG AtomicDecrement(LONG volatile *Target)
{
    LONG	Old;

    do {
        Old = *Target;

    }while (!AtomicCAS(Target, Old - 1, Old));

    return Old;
}

#endif

BOOL TAS(LONG volatile *value)
{
    LONG	ret;
    ret = AtomicCAS(value, 1, 0);
    if ( ret == 0 )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}




