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


/**	���г�ʼ��һ�������ľ��� a = { value };
    ���������Ԫ�ض����ڸ����ĳ�ʼֵ

    @param	T *a - Ҫ��ʼ���ľ���	
    @param	int row - ����	
    @param	int col - ����	
    @param	T value - ��ʼֵ	
    @return	void - ��	
*/
template <class T>
void Parallel_Matrix_Init(T *a, int row, int col, T value)
{
    int i;

    int size = row * col;

    if ( size < MIN_PARALLEL_EVALUATE_COUNT )
    {
        //���ô��еľ����ʼ������
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


/**	���н�һ������ֵ����һ������ a = b
    ����a����һԪ�ص��ھ���b��Ӧλ��Ԫ�ص�ֵ

    @param	T *a - ����ֵ�ľ���	
    @param	int row - ����	
    @param	int col - ����
    @param	int as - ����a��ԭʼ�����һ�д�С��������
    @param	T *b - ��ֵ�ľ���	
    @param	int bs - ����b��ԭʼ�����һ�д�С��������
    @return	void - ��	
*/
template <class T>
void Parallel_Matrix_Evaluate(T *a, int row, int col, int as, T *b, int bs)
{
    int i;

    if ( (row * col) < MIN_PARALLEL_EVALUATE_COUNT )
    {
        //���ô��еľ���ֵ����
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


/**	���н�һ�������ֵ�ӵ���һ�������� a = a + b
    ����a����һԪ�ص�ֵ������������Ͼ���b��Ӧλ��Ԫ�ص�ֵ

    @param	T *a - ����ӵľ���a��Ԫ��ֵ������Ӻ��ֵ	
    @param	int row_a - ����	
    @param	int col_a - ����
    @param	int as - ����a��ԭʼ�����һ�д�С��������
    @param	T *b - ��ӵľ��� b��Ԫ��ֵ����	
    @param	int bs - ����b��ԭʼ�����һ�д�С��������
    @return	void - ��	
*/
template <class T>
void Parallel_Matrix_AddSelf(T *a, int row_a, int col_a, int as,
                    T *b, int bs)
{
    int i;

    if ( (row_a * col_a) < MIN_PARALLEL_ADD_COUNT )
    {
        //���ô��еľ���ӷ�����
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


/**	���н�һ�������ֵ��ȥ��һ�������ֵ a = a - b
    ����a����һԪ�ص�ֵ�����������ȥ����b��Ӧλ��Ԫ�ص�ֵ

    @param	T *a - ������ľ���a��Ԫ��ֵ����������ֵ	
    @param	int row_a - ����	
    @param	int col_a - ����
    @param	int as - ����a��ԭʼ�����һ�д�С��������
    @param	T *b - ����ľ��� b��Ԫ��ֵ����	
    @param	int bs - ����b��ԭʼ�����һ�д�С��������
    @return	void - ��	
*/
template <class T>
void Parallel_Matrix_SubSelf(T *a, int row_a, int col_a, int as,
                    T *b, int bs)
{
    int i;
    
    if ( (row_a * col_a) < MIN_PARALLEL_SUB_COUNT )
    {
        // ���ô��о����������
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

/**	���н������������ c = a + b
    ����c����һԪ�ص�ֵ���ھ���a��b��Ӧλ��Ԫ�ص�ֵ�ĺ�

    @param	T *a - ��ӵľ���a��Ԫ��ֵ����	
    @param	int row_a - ����	
    @param	int col_a - ����
    @param	int as - ����a��ԭʼ�����һ�д�С��������
    @param	T *b - ��ӵľ��� b��Ԫ��ֵ����	
    @param	int bs - ����b��ԭʼ�����һ�д�С��������
    @param	T *c - ����ӵľ��� c��Ԫ��ֵ����a��b����λ��Ԫ��ֵ�ĺ�	
    @param	int cs - ����c��ԭʼ�����һ�д�С��������
    @return	void - ��	
*/
template <class T>
void Parallel_Matrix_Add(T *a, int row_a, int col_a, int as,
                T *b, int bs, T *c, int cs)
{
    int i;

    if ( (row_a * col_a) < MIN_PARALLEL_ADD_COUNT )
    {
        //���ô��еľ���ӷ�����
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

/**	���н������������ c = a - b
    ����c����һԪ�ص�ֵ���ھ���a��b��Ӧλ��Ԫ�ص�ֵ�Ĳ�

    @param	T *a - ������ľ���a��Ԫ��ֵ����	
    @param	int row_a - ����	
    @param	int col_a - ����
    @param	int as - ����a��ԭʼ�����һ�д�С��������
    @param	T *b - ����ľ��� b��Ԫ��ֵ����	
    @param	int bs - ����b��ԭʼ�����һ�д�С��������
    @param	T *c - ��Ž���ľ��� c��Ԫ��ֵ����a��b����λ��Ԫ��ֵ�Ĳ�	
    @param	int cs - ����c��ԭʼ�����һ�д�С��������
    @return	void - ��	
*/
template <class T>
void Parallel_Matrix_Sub(T *a, int row_a, int col_a, int as,
                T *b, int bs, T *c, int cs)
{
    int i;

    if ( (row_a * col_a) < MIN_PARALLEL_SUB_COUNT )
    {
        //���ô��еľ����������
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



/**	���н���������ֿ���� c = a * b
    ��������ʽ�ֿ鷨���о���ֿ����

    @param	T *a - ��˵ľ���a��Ԫ��ֵ����	
    @param	int row_a - a������	
    @param	int col_a - a������
    @param	int as - ����a��ԭʼ�����һ�д�С��������
    @param	T *b - ��˵ľ��� b��Ԫ��ֵ����	
    @param	int row_b - b������	
    @param	int col_b - b������
    @param	int bs - ����b��ԭʼ�����һ�д�С��������
    @param	T *c - ����ӵľ��� c��Ԫ��ֵ����a��b����λ��Ԫ��ֵ�ĺ�	
    @param	int cs - ����c��ԭʼ�����һ�д�С��������
    @param	int block_size - �ֳɵĿ�Ĵ�С��block_size * block_size)
                    ����block_size����64����ô�ֿ���Ӿ����СΪ64 �� 64

    @return	void - ��	
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

    int row_block_num;  //a���л��ֳɵĿ�����c���п�����
    int col_block_num;  //b���л��ֳɵĿ�����c���п�����
    int block_num;      //a����(b����)���ֳɵĿ���

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
            //�Ӿ���C[i][j] += A[i][k] * B[k][j];

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

            //Matrix_Multiply3()�л��Զ�ִ�н�a*b��ֵ�ۼӵ�c��
            Matrix_Multiply3(a, row_i, row_a_end, col_k, col_a_end, as,
                b, row_k, row_b_end, col_j, col_b_end, bs,
                c, row_i, col_j, cs );
        }
    }
}




/**	���о���˷�����

    @param	int *a - ָ��Ҫ��˵ĵ�1�������ָ��
    @param	int row_a - ����a������
    @param	int col_a - ����a������
    @param	int as - ����a��ԭʼ����������һ�еĴ�С��

    @param	int *b - ָ��Ҫ��˵ĵ�2�������ָ��	
    @param	int row_b - ����b������
    @param	int col_b - ����b������
    @param	int bs - ����b��ԭʼ����������һ�еĴ�С��

    @param	int *c - �������ľ����ָ��
    @param	int cs - ����c��ԭʼ����������һ�еĴ�С��

    @return	void - ��	
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