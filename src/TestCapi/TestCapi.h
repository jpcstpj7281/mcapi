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
//
// capitest.h
// capitest golbal definition.
//
#ifndef __CAPITEST_H__
#define __CAPITEST_H__


#define		FREE_MEM_FLAG	0xdddddddd
#define		NEW_MEM_FLAG	0xcdcdcdcd


#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
typedef   unsigned char  BYTE;
typedef   unsigned int   UINT;
char* itoa(int value, char *psz, int radix); 
#else
#include <windows.h>
#endif





#endif // __CAPITEST_H__
