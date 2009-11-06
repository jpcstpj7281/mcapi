/*
 * Copyright (c) 2009
 * Author: Weiming Zhou
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  
 */

/* 
 *	Parallel_Matrix.h
 *
 *	DESCRIPTION
 *		Interface to a Parallel Matrix operation.
 *
 *	HISTORY
 *      05-29-2009  create by zhouweiming. 
 *
 */

#ifndef __PARALLEL_MATRIX_H__
#define __PARALLEL_MATRIX_H__


#include "CapiGlobal.h"
#include "CapiCommon.h"
#include "Matrix.h"


#define  MIN_PARALLEL_EVALUATE_COUNT    20000
#define  MIN_PARALLEL_MULTIPLY_COUNT    8000
#define  MIN_PARALLEL_SUB_COUNT         20000
#define  MIN_PARALLEL_ADD_COUNT         20000


/**	并行初始化一个给定的矩阵 a = { value };
    矩阵的所有元素都等于给定的初始值

    @param	T *a - 要初始化的矩阵	
    @param	int row - 行数	
    @param	int col - 列数	
    @param	T value - 初始值	
    @return	void - 无	
*/
template <class T>
void Parallel_Matrix_Init(T *a, int row, int col, T value)
{
    int i;

    int size = row * col;

    if ( size < MIN_PARALLEL_EVALUATE_COUNT )
    {
        //调用串行的矩阵初始化函数
        Matrix_Init(a, row, col, value);
        return;
    }

    int nCore = omp_get_num_procs();

#pragma omp parallel for private(i) num_threads(nCore)
    for ( i = 0; i < size; i++ )
    {
        a[i] = value;
    }
}


/**	并行将一个矩阵赋值给另一个矩阵 a = b
    矩阵a的任一元素等于矩阵b对应位置元素的值

    @param	T *a - 被赋值的矩阵	
    @param	int row - 行数	
    @param	int col - 列数
    @param	int as - 矩阵a的原始矩阵的一行大小（列数）
    @param	T *b - 赋值的矩阵	
    @param	int bs - 矩阵b的原始矩阵的一行大小（列数）
    @return	void - 无	
*/
template <class T>
void Parallel_Matrix_Evaluate(T *a, int row, int col, int as, T *b, int bs)
{
    int i;

    if ( (row * col) < MIN_PARALLEL_EVALUATE_COUNT )
    {
        //调用串行的矩阵赋值函数
        Matrix_Evaluate(a, row, col, as, b, bs);
        return;
    }

    int nCore = omp_get_num_procs();

#pragma omp parallel for num_threads(nCore)
    for ( i = 0; i < row; i++ )
    {
        int j;
        int row_i_a = i * as;
        int row_i_b = i * bs;
        for ( j = 0; j < col; j++)
        {
            // a[i][j] = b[i][j]
            a[row_i_a + j] = b[row_i_b + j];
        }
    }
    return;
}


/**	并行将一个矩阵的值加到另一个矩阵上 a = a + b
    矩阵a的任一元素的值等于它自身加上矩阵b对应位置元素的值

    @param	T *a - 被相加的矩阵，a的元素值等于相加后的值	
    @param	int row_a - 行数	
    @param	int col_a - 列数
    @param	int as - 矩阵a的原始矩阵的一行大小（列数）
    @param	T *b - 相加的矩阵， b的元素值不变	
    @param	int bs - 矩阵b的原始矩阵的一行大小（列数）
    @return	void - 无	
*/
template <class T>
void Parallel_Matrix_AddSelf(T *a, int row_a, int col_a, int as,
                    T *b, int bs)
{
    int i;

    if ( (row_a * col_a) < MIN_PARALLEL_ADD_COUNT )
    {
        //调用串行的矩阵加法函数
        Matrix_AddSelf(a, row_a, col_a, as, b, bs);
        return;
    }


    int nCore = omp_get_num_procs();

#pragma omp parallel for num_threads(nCore)
    for ( i = 0; i < row_a; i++ )
    {
        int j;
        int row_i_a = i * as;
        int row_i_b = i * bs;
        for ( j = 0; j < col_a; j++ )
        {
            // a[i][j] = a[i][j] + b[i][j]
            a[row_i_a + j] += b[row_i_b + j];
        }
    }
}


/**	并行将一个矩阵的值减去另一个矩阵的值 a = a - b
    矩阵a的任一元素的值等于它自身减去矩阵b对应位置元素的值

    @param	T *a - 被相减的矩阵，a的元素值等于相减后的值	
    @param	int row_a - 行数	
    @param	int col_a - 列数
    @param	int as - 矩阵a的原始矩阵的一行大小（列数）
    @param	T *b - 相减的矩阵， b的元素值不变	
    @param	int bs - 矩阵b的原始矩阵的一行大小（列数）
    @return	void - 无	
*/
template <class T>
void Parallel_Matrix_SubSelf(T *a, int row_a, int col_a, int as,
                    T *b, int bs)
{
    int i;
    
    if ( (row_a * col_a) < MIN_PARALLEL_SUB_COUNT )
    {
        // 调用串行矩阵减法函数
        Matrix_SubSelf(a, row_a, col_a, as, b, bs);
        return;
    }

    int nCore = omp_get_num_procs();

#pragma omp parallel for num_threads(nCore)
    for ( i = 0; i < row_a; i++ )
    {
        int j;
        int row_i_a = i * as;
        int row_i_b = i * bs;
        for ( j = 0; j < col_a; j++ )
        {
            // a[i][j] = a[i][j] - b[i][j]
            a[row_i_a + j] -= b[row_i_b + j];
        }
    }
}

/**	并行将两个矩阵相加 c = a + b
    矩阵c的任一元素的值等于矩阵a、b对应位置元素的值的和

    @param	T *a - 相加的矩阵，a的元素值不变	
    @param	int row_a - 行数	
    @param	int col_a - 列数
    @param	int as - 矩阵a的原始矩阵的一行大小（列数）
    @param	T *b - 相加的矩阵， b的元素值不变	
    @param	int bs - 矩阵b的原始矩阵的一行大小（列数）
    @param	T *c - 被相加的矩阵， c的元素值等于a、b对于位置元素值的和	
    @param	int cs - 矩阵c的原始矩阵的一行大小（列数）
    @return	void - 无	
*/
template <class T>
void Parallel_Matrix_Add(T *a, int row_a, int col_a, int as,
                T *b, int bs, T *c, int cs)
{
    int i;

    if ( (row_a * col_a) < MIN_PARALLEL_ADD_COUNT )
    {
        //调用串行的矩阵加法函数
        Matrix_Add(a, row_a, col_a, as, b, bs, c, cs);
        return;
    }

    int nCore = omp_get_num_procs();

#pragma omp parallel for private(i) num_threads(nCore)
    for ( i = 0; i < row_a; i++ )
    {
        int j;
        int row_i_a = i * as;
        int row_i_b = i * bs;
        int row_i_c = i * cs;
        for ( j = 0; j < col_a; j++ )
        {
            // c[i][j] = a[i][j] - b[i][j]
            c[row_i_c + j] = a[row_i_a + j] + b[row_i_b + j];
        }
    }
}

/**	并行将两个矩阵相减 c = a - b
    矩阵c的任一元素的值等于矩阵a、b对应位置元素的值的差

    @param	T *a - 被相减的矩阵，a的元素值不变	
    @param	int row_a - 行数	
    @param	int col_a - 列数
    @param	int as - 矩阵a的原始矩阵的一行大小（列数）
    @param	T *b - 相减的矩阵， b的元素值不变	
    @param	int bs - 矩阵b的原始矩阵的一行大小（列数）
    @param	T *c - 存放结果的矩阵， c的元素值等于a、b对于位置元素值的差	
    @param	int cs - 矩阵c的原始矩阵的一行大小（列数）
    @return	void - 无	
*/
template <class T>
void Parallel_Matrix_Sub(T *a, int row_a, int col_a, int as,
                T *b, int bs, T *c, int cs)
{
    int i;

    if ( (row_a * col_a) < MIN_PARALLEL_SUB_COUNT )
    {
        //调用串行的矩阵减法函数
        Matrix_Sub(a, row_a, col_a, as, b, bs, c, cs);
        return;
    }

    const int nCore = omp_get_num_procs();

#pragma omp parallel for private(i) num_threads(nCore)
    for ( i = 0; i < row_a; i++ )
    {
        int j;
        int row_i_a = i * as;
        int row_i_b = i * bs;
        int row_i_c = i * cs;
        for ( j = 0; j < col_a; j++ )
        {
            //c[i][j] = a[i][j] - b[i][j]
            c[row_i_c + j] = a[row_i_a + j] - b[row_i_b + j];
        }
    }
}



/**	并行将两个矩阵分块相乘 c = a * b
    按照棋盘式分块法进行矩阵分块相乘

    @param	T *a - 相乘的矩阵，a的元素值不变	
    @param	int row_a - a的行数	
    @param	int col_a - a的列数
    @param	int as - 矩阵a的原始矩阵的一行大小（列数）
    @param	T *b - 相乘的矩阵， b的元素值不变	
    @param	int row_b - b的行数	
    @param	int col_b - b的列数
    @param	int bs - 矩阵b的原始矩阵的一行大小（列数）
    @param	T *c - 被相加的矩阵， c的元素值等于a、b对于位置元素值的和	
    @param	int cs - 矩阵c的原始矩阵的一行大小（列数）
    @param	int block_size - 分成的块的大小（block_size * block_size)
                    比如block_size等于64，那么分块的子矩阵大小为64 × 64

    @return	void - 无	
*/
template <class T>
void Parallel_Blocked_Matrix_Multiply(T *a, int row_a, int col_a, int as,
                                      T *b, int row_b, int col_b, int bs,
                                      T *c, int cs, int block_size)
{
    if ( col_a != row_b )
    {
        //assert(0);    
        return;
    }

    if ( (row_a * col_b) < (MIN_PARALLEL_MULTIPLY_COUNT / col_a) )
    {
        Blocked_Matrix_Multiply(a, row_a, col_a, as, b, row_b, col_b, bs, c, cs, block_size);
        return;
    }

    int row_block_num;  //a的行划分成的块数（c的行块数）
    int col_block_num;  //b的列划分成的块数（c的列块数）
    int block_num;      //a的列(b的行)划分成的块数

    row_block_num = row_a / block_size;
    if ( (row_a % block_size) != 0 )
    {
        row_block_num += 1;
    }
    col_block_num = col_b / block_size;
    if ( (col_b % block_size) != 0 )
    {
        col_block_num += 1;
    }

    block_num = col_a / block_size;
    if ( (col_a % block_size) != 0 )
    {
        block_num += 1;
    }

    int row = block_size;
    int col = block_size;
    int index;
    int border = row_block_num * col_block_num;

    Parallel_Matrix_Init(c, row_a, col_b, (T)0);

    int nCore = omp_get_num_procs();
#pragma omp parallel for private(index) num_threads(nCore)
    for ( index = 0; index < border; index++ )
    {
        int i, j, k;

        i = index / col_block_num;
        j = index % col_block_num;

        int row_i = i * row;
        int row_a_end = row_i + row;
        if ( row_a_end >= row_a )
        {
            row_a_end = row_a;
        }

        int col_j = j * col;
        int col_b_end = col_j + col;
        if ( col_b_end >= col_b )
        {
            col_b_end = col_b;
        }

        for ( k = 0; k < block_num; k++ )
        {
            //子矩阵C[i][j] += A[i][k] * B[k][j];

            int row_k = k * row;
            int col_k = k * col;

            int col_a_end = col_k + col;
            if ( col_a_end >= col_a )
            {
                col_a_end = col_a;
            }

            int row_b_end = row_k + row;
            if ( row_b_end >= row_b )
            {
                row_b_end = row_b;
            }

            //Matrix_Multiply3()中会自动执行将a*b的值累加到c上
            Matrix_Multiply3(a, row_i, row_a_end, col_k, col_a_end, as,
                b, row_k, row_b_end, col_j, col_b_end, bs,
                c, row_i, col_j, cs );
        }
    }
}




/**	并行矩阵乘法函数

    @param	int *a - 指向要相乘的第1个矩阵的指针
    @param	int row_a - 矩阵a的行数
    @param	int col_a - 矩阵a的列数
    @param	int as - 矩阵a的原始矩阵列数（一行的大小）

    @param	int *b - 指向要相乘的第2个矩阵的指针	
    @param	int row_b - 矩阵b的行数
    @param	int col_b - 矩阵b的列数
    @param	int bs - 矩阵b的原始矩阵列数（一行的大小）

    @param	int *c - 计算结果的矩阵的指针
    @param	int cs - 矩阵c的原始矩阵列数（一行的大小）

    @return	void - 无	
*/
template <class T>
void Parallel_Matrix_Multiply(T *a, int row_a, int col_a,int as, 
                              T *b, int row_b, int col_b, int bs,
                              T *c, int cs )
{
    if ( col_a != row_b )
    {
        return;
    }
    
    int index;
    int border = row_a * col_b;

    if ( (border * col_a) < MIN_PARALLEL_MULTIPLY_COUNT )
    {
        Matrix_Multiply(a, row_a, col_a, as, b, row_b, col_b, bs, c, cs);
        return;
    }

#pragma omp parallel for  num_threads(dtn(border, 1))
    for ( index = 0; index < border; index++ )
    {
        int i, j, k; 

        i = index / col_b;
        j = index % col_b;

        int row_i = i * as;
        int row_c = i * cs;

        c[row_c+j] = 0;
        for ( k = 0; k < row_b; k++ )
        {
            // c[i][j] += a[i][k] * b[k][j]
            c[row_c + j] += a[row_i + k] * b[k * bs + j];
        }
    }
}



#endif //__PARALLEL_MATRIX_H__