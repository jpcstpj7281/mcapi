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
 *	Matrix.h
 *
 *	DESCRIPTION
 *		Interface to a Matrix operation.
 *
 *	HISTORY
 *		05-06-2008	create by zhouweiming.
 *      05-29-2009  ReWrite use template class 
 *
 */
#ifndef __MATRIX_H__
#define __MATRIX_H__


/*
template <class T>
void Matrix_Init(T *a, int row, int col, T value);
template <class T>
void Matrix_Evaluate(T *a, int row, int col, int as, T *b, int bs);


template <class T>
void Matrix_AddSelf(T *a, int row_a, int col_a, int as, T *b, int bs);
template <class T>
void Matrix_SubSelf(T *a, int row_a, int col_a, int as, T *b, int bs);

template <class T>
void Matrix_Add(T *a, int row_a, int col_a, int as,
                T *b, int bs, T *c, int cs);
template <class T>
void Matrix_Sub(T *a, int row_a, int col_a, int as,
                T *b, int bs, T *c, int cs);



template <class T>
int Matrix_Turn(T *a, int row_a, int col_a, T *b, int b_size);
template <class T>
int Matrix_Turn2(T *a, int row_a_begin, int row_a_end, 
                 int col_a_begin, int col_a_end, 
                 int row_a, int col_a,
                 T *b, int b_size);


template <class T>
void Matrix_Multiply(T *a, int row_a, int col_a,int as, 
                     T *b, int row_b, int col_b, int bs,
                     T *c, int cs );
template <class T>
void Matrix_Multiply2(T *a, int row_a, int col_a, 
                      T *b, int row_b, int col_b,
                      T *c, int c_size);
template <class T>
void Matrix_Multiply3(T *a, int row_a_begin, int row_a_end,
                      int col_a_begin, int col_a_end, 
                      int row_a, int col_a,
                      T *b, int row_b_begin, int row_b_end,
                      int col_b_begin, int col_b_end,
                      int row_b, int col_b,
                      T *c, int row_c_begin, int col_c_begin);
template <class T>
void Blocked_Matrix_Multiply(T *a, int row_a, int col_a, int as,
                             T *b, int row_b, int col_b, int bs,
                             T *c, int cs, int block_size);


template <class T>
void Matrix_Print(T * a, int row, int col);

template <class T>
void Matrix_Print2(T * a, int row, int col, int as);
*/


#define     MIN_MATRIX_ELEMENT_COUNT    200
#define     DEF_MATRIX_BLOCK_SIZE       64


/**	��ʼ��һ�������ľ��� a = { value };
    ���������Ԫ�ض����ڸ����ĳ�ʼֵ

    @param	T *a - Ҫ��ʼ���ľ���	
    @param	int row - ����	
    @param	int col - ����	
    @param	T value - ��ʼֵ	
    @return	void - ��	
*/
template <class T>
void Matrix_Init(T *a, int row, int col, T value)
{
    int i;

    int size = row * col;

    for ( i = 0; i < size; i++ )
    {
        a[i] = value;
    }
}


/**	��һ������ֵ����һ������ a = b
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
void Matrix_Evaluate(T *a, int row, int col, int as, T *b, int bs)
{
    int i, j;

    for ( i = 0; i < row; i++ )
    {
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


/**	��һ�������ֵ�ӵ���һ�������� a = a + b
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
void Matrix_AddSelf(T *a, int row_a, int col_a, int as,
                    T *b, int bs)
{
    int i, j;

    for ( i = 0; i < row_a; i++ )
    {
        int row_i_a = i * as;
        int row_i_b = i * bs;
        for ( j = 0; j < col_a; j++ )
        {
            // a[i][j] = a[i][j] + b[i][j]
            a[row_i_a + j] += b[row_i_b + j];
        }
    }
}


/**	��һ�������ֵ��ȥ��һ�������ֵ a = a - b
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
void Matrix_SubSelf(T *a, int row_a, int col_a, int as,
                    T *b, int bs)
{
    int i, j;
    
    for ( i = 0; i < row_a; i++ )
    {
        int row_i_a = i * as;
        int row_i_b = i * bs;
        for ( j = 0; j < col_a; j++ )
        {
            // a[i][j] = a[i][j] - b[i][j]
            a[row_i_a + j] -= b[row_i_b + j];
        }
    }
}

/**	������������� c = a + b
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
void Matrix_Add(T *a, int row_a, int col_a, int as,
                T *b, int bs, T *c, int cs)
{
    int i, j;

    for ( i = 0; i < row_a; i++ )
    {
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

/**	������������� c = a - b
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
void Matrix_Sub(T *a, int row_a, int col_a, int as,
                T *b, int bs, T *c, int cs)
{
    int i, j;

    for ( i = 0; i < row_a; i++ )
    {
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



template <class T>
int Matrix_Turn(T *a, int row_a, int col_a, T *b, int b_size)
{
    if ( b_size != row_a * col_a )
    {
        return -1;
    }

    int i, j;
    for ( i = 0; i < col_a; i++ )
    {
        int row_i = i * row_a;
        for ( j = 0; j < row_a; j++)
        {
            b[row_i + j] = a[j * col_a + i];
        }
    }
    return 0;
}

template <class T>
void Matrix_Turn2(T *a, int row_a_begin, int row_a_end, 
                  int col_a_begin, int col_a_end, 
                  int as,
                  T *b)
{
    int row_size = row_a_end - row_a_begin;

    int i, j;
    for ( i = col_a_begin; i < col_a_end; i++ )
    {
        int row_i = (i - col_a_begin) * row_size;
        for ( j = row_a_begin; j < row_a_end; j++)
        {
            b[row_i + j - row_a_begin] = a[j * as + i];
        }
    }
    return;
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
void Matrix_Multiply(T *a, int row_a, int col_a,int as, 
                              T *b, int row_b, int col_b, int bs,
                              T *c, int cs )
{
    if ( col_a != row_b )
    {
        return;
    }

    int index;
    int border = row_a * col_b;

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


template <class T>
void Matrix_Multiply2(T *a, int row_a, int col_a, int as,
                      T *b, int row_b, int col_b, int bs,
                      T *c, int cs)
{
    int i, j, k;
    if ( col_a != row_b )
    {
        return;
    }
    if ( row_a == 0 || col_b == 0 || col_a == 0 )
    {
        return;
    }

    int b_size = row_b * col_b;

    T *d = new T[b_size];

    Matrix_Turn2(b, 0, row_b, 0, col_b, bs, d);

    for ( i = 0; i < row_a; i++ )
    {
        int row_i = i * as;
        int row_c = i * cs;
        for ( j = 0; j < col_b; j++ )
        {
            int row_j = j * row_b;
            T   sum = (T)0;
            for ( k = 0; k < row_b; k++ )
            {
                sum += a[row_i + k] * d[row_j + k];
            }
            c[row_c + j] = sum;
        }
    }
    delete [] d;
    return;
}

template <class T>
void Matrix_MultiplyNN(T *a, T *b, int n, int as, int bs, 
                       T *c, int cs)
{
    int i, j, k;


    int b_size = n * n;

    T *d = new T[b_size];

    Matrix_Turn2(b, 0, n, 0, n, bs, d);

    for ( i = 0; i < n; i++ )
    {
        int row_i_a = i * as;
        int row_i_c = i * cs;
        for ( j = 0; j < n; j++ )
        {
            int row_j = j * n;
            T   sum = (T)0;
            for ( k = 0; k < n; k++ )
            {
                sum += a[row_i_a + k] * d[row_j + k];
            }
            c[row_i_c + j] = sum;
        }
    }
    delete [] d;
    return;
}



template <class T>
void Matrix_Multiply3(T *a, int row_a_begin, int row_a_end,
                      int col_a_begin, int col_a_end, 
                      int as,
                      T *b, int row_b_begin, int row_b_end,
                      int col_b_begin, int col_b_end,
                      int bs,
                      T *c, int row_c_begin, int col_c_begin, int cs)
{
    int i, j, k;

    int col_a_size = col_a_end - col_a_begin;
    int row_b_size = row_b_end - row_b_begin;

    if ( col_a_size != row_b_size )
    {
        return;
    }

    int col_b_size = col_b_end - col_b_begin;
    int b_size = row_b_size * col_b_size;

    T *d = new T[b_size];

    Matrix_Turn2(b, row_b_begin, row_b_end, col_b_begin, col_b_end,
        bs, d);

    for ( i = row_a_begin; i < row_a_end; i++ )
    {
        int row_i = i * as;
        int row_c = (i - row_a_begin + row_c_begin) * cs;
        for ( j = 0; j < col_b_size; j++ )
        {
            int row_j = j * row_b_size;
            T   sum = (T)0;
            for ( k = col_a_begin; k < col_a_end; k++ )
            {
                sum += a[row_i + k] * d[row_j + k - col_a_begin];
            }
            c[row_c + j + col_c_begin] += sum;
        }
    }
    delete [] d;
    return;
}

/**	����������ֿ���� c = a * b
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
void Blocked_Matrix_Multiply(T *a, int row_a, int col_a, int as,
                                      T *b, int row_b, int col_b, int bs,
                                      T *c, int cs, int block_size)
{
    int nBlockSize;
    if ( col_a != row_b )
    {
        //assert(0);    
        return;
    }

    nBlockSize = block_size;
    if ( nBlockSize <= 0 )
    {
        nBlockSize = DEF_MATRIX_BLOCK_SIZE;
    }

    int i, j, k;
    int row_block_num;  //a���л��ֳɵĿ�����c���п�����
    int col_block_num;  //b���л��ֳɵĿ�����c���п�����
    int block_num;      //a����(b����)���ֳɵĿ���

    row_block_num = row_a / nBlockSize;
    if ( (row_a % nBlockSize) != 0 )
    {
        row_block_num += 1;
    }
    col_block_num = col_b / nBlockSize;
    if ( (col_b % nBlockSize) != 0 )
    {
        col_block_num += 1;
    }

    block_num = col_a / nBlockSize;
    if ( (col_a % nBlockSize) != 0 )
    {
        block_num += 1;
    }

    int row = nBlockSize;
    int col = nBlockSize;
    int index;
    int border = row_block_num * col_block_num;

    Matrix_Init(c, row_a, col_b, (T)0);

    for ( index = 0; index < border; index++ )
    {
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




template <class T>
void Matrix_Print(T * a, int row, int col)
{
    int i, j;
    for ( i = 0; i < row; i++ )
    {
        for ( j = 0; j < col; j++ )
        {
            printf("a[%d][%d]=%.1lf, ", i, j, (float)a[i*col +j]);
        }
        printf("\n");
    }
}


template <class T>
void Matrix_Print2(T * a, int row, int col, int as)
{
    int i, j;
    for ( i = 0; i < row; i++ )
    {
        for ( j = 0; j < col; j++ )
        {
            printf("a[%d][%d]=%.1lf, ", i, j, (float)a[i*as +j]);
        }
        printf("\n");
    }
}


/**	��һ���������һ��������бȽ�

    @param	T *a - ���Ƚϵľ���	
    @param	int row - ����	
    @param	int col - ����
    @param	int as - ����a��ԭʼ�����һ�д�С��������
    @param	T *b - ���Ƚϵľ���	
    @param	int bs - ����b��ԭʼ�����һ�д�С��������
    @return	void - ��	
*/
template <class T>
void Matrix_Compare(T *a, int row, int col, int as, T *b, int bs)
{
    int i, j;

    for ( i = 0; i < row; i++ )
    {
        int row_i_a = i * as;
        int row_i_b = i * bs;
        for ( j = 0; j < col; j++)
        {
            // �ж��Ƿ� a[i][j] == b[i][j]
            if ( a[row_i_a + j] != b[row_i_b + j] )
            {
                printf("a[%d][%d] = %.1lf != b[%d][%d] = %.1lf\n", 
                    i, j, (float)a[row_i_a + j], i, j, (float)b[row_i_b + j] );
            }
        }
    }
    return;
}


#endif /* __MATRIX_H__ */