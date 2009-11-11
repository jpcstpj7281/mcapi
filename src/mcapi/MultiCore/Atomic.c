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
#include <unistd.h>
#include <sys/types.h>
#endif

#include "CapiGlobal.h"
#include "Atomic.h"

#if _WIN32 

BOOL AtomicCAS(LONG volatile *dest, LONG newvalue, LONG oldvalue)
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

BOOL AtomicCAS64(LONGLONG volatile *dest, LONGLONG newvalue, LONGLONG oldvalue)
{
    /* not implement yet */
    return FALSE;
}
#if 0
BOOL AtomicCAS64(LONGLONG volatile *dest, LONGLONG newvalue, LONGLONG oldvalue)
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

#else /* For Linux */ 

int AtomicCAS(LONG volatile *dest, LONG newvalue, LONG oldvalue) 
{

    LONG ret;

    __asm __volatile ("lock; cmpxchgl %2, %1"

        : "=a" (ret), "=m" (*dest)

        : "r" (newvalue), "m" (*dest), "0" (oldvalue));

    if ( ret == oldvalue )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


#if 0
int AtomicCAS(volatile void *dest, int newvalue, int oldvalue ) 
{                                                                                   
    int result;                                                                       

    __asm__ __volatile__("lock\ncmpxchgl %2,%1"                                     
        : "=a" (result), "=m" (*(int32_t *)dest)                                           
        : "q" (newvalue), "0"(oldvalue)               
        : "memory");                                             
    return result;                                                                
}   
#endif

union {
    int64_t comparand_local;
    int32_t comparand_parts[2];
} INT64_U;

int64_t AtomicCAS64(volatile void *dest, int64_t newvalue, int64_t oldvalue )
{
    int64_t result;
    INT64_U tmp;

    tmp.comparand_local = oldvalue;
    /* EBX register saved for compliancy with position-independent code (PIC) rules on IA32 */
    __asm__ __volatile__ (
        "pushl %%ebx\n\t"
        "movl  (%%ecx),%%ebx\n\t"
        "movl  4(%%ecx),%%ecx\n\t"
        "lock\ncmpxchg8b (%2)\n\t"
        "popl  %%ebx"
        : "=A"(result), "=m"(*(int64_t *)dest)
        : "S"(dest),
        "a"(tmp.comparand_parts[0]),
        "d"(tmp.comparand_parts[1]),
        "c"(&newvalue)
        : "memory", "esp");

    if ( result == oldvalue )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
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

    return (Old + 1);
}


LONG AtomicDecrement(LONG volatile *Target)
{
    LONG	Old;

    do {
        Old = *Target;

    }while (!AtomicCAS(Target, Old - 1, Old));

    return (Old - 1);
}

#endif /* ifndef _WIN32 */

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




