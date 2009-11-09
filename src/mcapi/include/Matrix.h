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


/**	初始化一个给定的矩阵 a = { value };
    矩阵的所有元素都等于给定的初始值

    @param	T *a - 要初始化的矩阵	
    @param	int row - 行数	
    @param	int col - 列数	
    @param	T value - 初始值	
    @return	void - 无	
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


/**	将一个矩阵赋值给另一个矩阵 a = b
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


/**	将一个矩阵的值加到另一个矩阵上 a = a + b
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


/**	将一个矩阵的值减去另一个矩阵的值 a = a - b
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

/**	将两个矩阵相加 c = a + b
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

/**	将两个矩阵相减 c = a - b
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


/**	串行矩阵乘法函数

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

/**	将两个矩阵分块相乘 c = a * b
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
    int row_block_num;  //a的行划分成的块数（c的行块数）
    int col_block_num;  //b的列划分成的块数（c的列块数）
    int block_num;      //a的列(b的行)划分成的块数

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


/**	将一个矩阵和另一个矩阵进行比较

    @param	T *a - 被比较的矩阵	
    @param	int row - 行数	
    @param	int col - 列数
    @param	int as - 矩阵a的原始矩阵的一行大小（列数）
    @param	T *b - 被比较的矩阵	
    @param	int bs - 矩阵b的原始矩阵的一行大小（列数）
    @return	void - 无	
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
            // 判断是否 a[i][j] == b[i][j]
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