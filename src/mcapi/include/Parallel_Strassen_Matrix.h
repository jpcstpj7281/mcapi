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
 *	Parallel_Strassen_Matrix.h
 *
 *	DESCRIPTION
 *		Interface to a Parallel Strassen Matrix Mulitplication operation.
 *
 *	HISTORY
 *      05-29-2009  create by zhouweiming. 
 *
 */

#ifndef __PARALLEL_STRASSEN_MATRIX_H__
#define __PARALLEL_STRASSEN_MATRIX_H__

#include "CWaitTaskScheduler.h"
#include "Strassen_Matrix.h"

#define MIN_PARALLEL_STRASSEN_MATRIX_SIZE   128

template <class T>
class CStrassenParam {
public:
    T   *m_a;
    int m_row_a;
    int m_col_a;
    int m_as;
    T   *m_b;
    int m_row_b;
    int m_col_b;
    int m_bs;
    T   *m_c;
    int m_cs;
    int m_depth;
    int m_max_depth;

    int m_strassen_size;

    CWaitTaskScheduler  *m_pSched;

    CStrassenParam()
    {
        m_depth = 0;
        m_max_depth = DEF_STRASSEN_DEPTH;
        m_strassen_size = MIN_PARALLEL_STRASSEN_MATRIX_SIZE;
    };

    void SetPara(T *a, int row_a, int col_a, int as, 
        T *b, int row_b, int col_b, int bs,
        T *c, int cs, int depth, int max_depth, 
        int strassen_size,
        CWaitTaskScheduler *pSched);
};

template <class T>
void CStrassenParam<T>::SetPara(T *a, int row_a, int col_a, int as, 
                         T *b, int row_b, int col_b, int bs, 
                         T *c, int cs, int depth, int max_depth, 
                         int strassen_size,
                         CWaitTaskScheduler *pSched)
{
    m_a =a;
    m_row_a = row_a;
    m_col_a = col_a;
    m_as = as;

    m_b = b;
    m_row_b = row_b;
    m_col_b = col_b;
    m_bs = bs;

    m_c = c;
    m_cs = cs;

    m_depth = depth;
    m_max_depth = max_depth;
    m_strassen_size = strassen_size;
    m_pSched = pSched;
}


/**	并行Strassen矩阵乘法的任务处理入口函数
    负责进行任务拆分，当任务不能继续拆分时，调用串行Strassen乘法进行计算

    C = A * B
    先将A、B、C拆分成以下形式：
    A = { A00  A01}     B = {B00  B01}    C = {C00  C01}
        { A10  A11}         {B10  B11}        {C10  C11}

    其中A00,B00,C00都是n×n大小的矩阵（n为偶数）
    这样就求C时按以下方法求解
    C00 = A00 * B00 + A01 * B10
    C01 = A00 * B01 + A01 * B11
    C10 = A10 * B00 + A11 * B10
    C11 = A10 * B01 + A11 * B11

    总共有8个矩阵乘法，先求解A00*B00，按Strassen方法将A00,B00,C00
    分解成4个大小相等的矩阵如下：
    A00 = { A0  A1}     B00 = {B0  B1}    C00 = {C0  C1}
          { A2  A3}         {B2  B3}        {C2  C3}

    然后按以下计算过程计算
    M1 = (A0 + A3) * (B0 + B3)
    M2 = (A2 + A3) * B0
    M3 = A0 * (B1 - B3)
    M4 = A3 * (B2 - B0)
    M5 = (A0 + A1) * B3
    M6 = (A2 - A0) * (B0 + B1)
    M7 = (A1 - A3) * (B2 + B3)
    以上求M1~M7的7次乘法可以划作7个任务，在任务中进行乘法计算，
    放入任务调度器中进行调度运行，等7个任务全部完成后，再进行下面的计算

    C0 = M1 + M4 - M5 + M7
    C1 = M3 + M5
    C2 = M2 + M4
    C3 = M1 - M2 + M3 + M6

    计算出C0 C1 C2 C3后，就得到了A00*B00的值,然后A01*B10, A00*B01等另外7个
    矩阵乘法同样当作7个任务，进行调度运行并等全部执行完后，最后就可求出
    C00、C01、C10、C11的值，这样就求出了矩阵C＝A*B


	@param	void *pArg - 实际为一个CStrassenParam指针	
	@return	unsigned int WINAPI - 返回CAPI_FAILED表示失败，返回CAPI_SUCCESS表示成功	
*/
template <class T>
unsigned int WINAPI  Strassen_Matrix_Task(LPVOID pArg)
{
    CStrassenParam<T> *pPara = (CStrassenParam<T> *)pArg;

    if ( pPara == NULL )
    {
        return CAPI_FAILED;
    }

    T *a = pPara->m_a;
    int row_a = pPara->m_row_a;
    int row_b = pPara->m_row_b;
    int as = pPara->m_as;

    T *b = pPara->m_b;
    int col_a = pPara->m_col_a;
    int col_b = pPara->m_col_b;
    int bs = pPara->m_bs;

    T *c = pPara->m_c;
    int cs = pPara->m_cs;
    int depth = pPara->m_depth;
    int max_depth = pPara->m_max_depth;
    CWaitTaskScheduler *pSched = pPara->m_pSched;
    int ss = pPara->m_strassen_size;

    int n;

    n = row_a;

    if ( n > row_b )
    {
        n = row_b;
    }
    if ( n > col_b )
    {
        n = col_b;
    }

    //将n转成偶数
    if ( (n & 0x1) == 1 )
    {
        n -= 1;
    }

    if ( n <= ss || depth >= max_depth)
    {
        Strassen_Matrix_Multiply(a, row_a, col_a, as,
            b, row_b, col_b, bs, c, cs, 0);

        return CAPI_SUCCESS;
    }


    T *A00, *A01, *A10, *A11;
    T *B00, *B01, *B10, *B11;
    T *C00, *C01, *C10, *C11;

    A00 = a;
    A01 = a + n;
    A10 = a + n * as;
    A11 = a + n * as + n;

    B00 = b;
    B01 = b + n;
    B10 = b + n * bs;
    B11 = b + n * bs + n;

    C00 = c;
    C01 = c + n;
    C10 = c + n * cs;
    C11 = c + n * cs + n;

    int n_2 = n >> 1;

    T * T1 = new T[n_2 * n_2];
    T * T2 = new T[n_2 * n_2];
    T * T3 = new T[n_2 * n_2];
    T * T4 = new T[n_2 * n_2];
    T * T5 = new T[n_2 * n_2];
    T * T6 = new T[n_2 * n_2];
    T * T7 = new T[n_2 * n_2];
    T * T8 = new T[n_2 * n_2];
    T * T9 = new T[n_2 * n_2];
    T * T10 = new T[n_2 * n_2];

    T * M1 = new T[n_2 * n_2];
    T * M2 = new T[n_2 * n_2];
    T * M3 = new T[n_2 * n_2];
    T * M4 = new T[n_2 * n_2];
    T * M5 = new T[n_2 * n_2];
    T * M6 = new T[n_2 * n_2];
    T * M7 = new T[n_2 * n_2];


    T *A0, *A1, *A2, *A3;
    T *B0, *B1, *B2, *B3;
    T *C0, *C1, *C2, *C3;

    // A00 = {A0  A1}
    //       {A2  A3}
    A0 = a;
    A1 = a + n_2;
    A2 = a + n_2 * as;
    A3 = a + n_2 * as + n_2;

    // B00 = {B0  B1}
    //       {B2  B3}
    B0 = b;
    B1 = b + n_2;
    B2 = b + n_2 * bs;
    B3 = b + n_2 * bs + n_2;

    // C00 = {C0  C1}
    //       {C2  C3}
    C0 = c;
    C1 = c + n_2;
    C2 = c + n_2 * cs;
    C3 = c + n_2 * cs + n_2;

    //
    //下面的代码计算A00*B00，将其拆分成7次矩阵相乘
    //
    CTaskArray  task_array(7);

    //T1=B1 - B3
    //M3=A0*T1 放到task[0]里实现
    Matrix_Sub(B1, n_2, n_2, bs, B3, bs, T1, n_2);

    CStrassenParam<T> * pPara1 = new CStrassenParam<T>;
    pPara1->SetPara(A0, n_2, n_2, as, T1, n_2, n_2, n_2, 
        M3, n_2, depth+1, max_depth, ss, pSched);
    task_array.AddTask(Strassen_Matrix_Task<T>, pPara1);

    //T2=A2 - A0 
    //T3=B0 + B1 
    //M6=T2*T3
    Matrix_Sub(A2, n_2, n_2, as, A0, as, T2, n_2);
    Matrix_Add(B0, n_2, n_2, bs, B1, bs, T3, n_2);

    pPara1 = new CStrassenParam<T>;
    pPara1->SetPara(T2, n_2, n_2, n_2, T3, n_2, n_2, n_2, 
        M6, n_2, depth+1, max_depth, ss, pSched);
    task_array.AddTask(Strassen_Matrix_Task<T>, pPara1);

    //T4=A2 + A3 
    //M2=T4*B0 
    Matrix_Add(A2, n_2, n_2, as, A3, as, T4, n_2);

    pPara1 = new CStrassenParam<T>;
    pPara1->SetPara(T4, n_2, n_2, n_2, B0, n_2, n_2, bs, 
        M2, n_2, depth+1, max_depth, ss, pSched);
    task_array.AddTask(Strassen_Matrix_Task<T>, pPara1);

    //T5=A0 + A3 
    //T6=B0 + B3 
    //M1=T5*T6 
    Matrix_Add(A0, n_2, n_2, as, A3, as, T5, n_2);
    Matrix_Add(B0, n_2, n_2, bs, B3, bs, T6, n_2);

    pPara1 = new CStrassenParam<T>;
    pPara1->SetPara(T5, n_2, n_2, n_2, T6, n_2, n_2, n_2, 
        M1, n_2, depth+1, max_depth, ss, pSched);
    task_array.AddTask(Strassen_Matrix_Task<T>, pPara1);

    //T7=A0 + A1 
    //M5=T7*B3 
    Matrix_Add(A0, n_2, n_2, as, A1, as, T7, n_2);

    pPara1 = new CStrassenParam<T>;
    pPara1->SetPara(T7, n_2, n_2, n_2, B3, n_2, n_2, bs, 
        M5, n_2, depth+1, max_depth, ss, pSched);
    task_array.AddTask(Strassen_Matrix_Task<T>, pPara1);

    //T8=A1 - A3 
    //T9=B2 + B3 
    //M7=T8*T9 
    Matrix_Sub(A1, n_2, n_2, as, A3, as, T8, n_2);
    Matrix_Add(B2, n_2, n_2, bs, B3, bs, T9, n_2);

    pPara1 = new CStrassenParam<T>;
    pPara1->SetPara(T8, n_2, n_2, n_2, T9, n_2, n_2, n_2, 
        M7, n_2, depth+1, max_depth, ss, pSched);
    task_array.AddTask(Strassen_Matrix_Task<T>, pPara1);

    //T10=B2 - B0
    //M4=A3*T2 
    Matrix_Sub(B2, n_2, n_2, bs, B0, bs, T10, n_2);

    pPara1 = new CStrassenParam<T>;
    pPara1->SetPara(A3, n_2, n_2, as, T10, n_2, n_2, n_2, 
        M4, n_2, depth+1, max_depth, ss, pSched);
    task_array.AddTask(Strassen_Matrix_Task<T>, pPara1);

    //
    //对任务组中的所有任务进行调度执行，并等待所有任务执行完
    //
    pPara->m_pSched->SpawnTaskArrayAndWait(&task_array);

    if ( depth >= 0 )
    {
        //C0 = M1 + M4 - M5 + M7
        Matrix_Evaluate(C0, n_2, n_2, cs, M1, n_2);
        Matrix_AddSelf(C0, n_2, n_2, cs, M4, n_2);
        Matrix_SubSelf(C0, n_2, n_2, cs, M5, n_2);
        Matrix_AddSelf(C0, n_2, n_2, cs, M7, n_2);

        //C1 = M3 + M5
        Matrix_Evaluate(C1, n_2, n_2, cs, M3, n_2);
        Matrix_AddSelf(C1, n_2, n_2, cs, M5, n_2);

        //C2 = M2 + M4
        Matrix_Evaluate(C2, n_2, n_2, cs, M2, n_2);
        Matrix_AddSelf(C2, n_2, n_2, cs, M4, n_2);

        //C3 = M1 - M2 + M3 + M6
        Matrix_Evaluate(C3, n_2, n_2, cs, M1, n_2);
        Matrix_SubSelf(C3, n_2, n_2, cs, M2, n_2);
        Matrix_AddSelf(C3, n_2, n_2, cs, M3, n_2);
        Matrix_AddSelf(C3, n_2, n_2, cs, M6, n_2);
    }
    else
    {
        //经测试,如果将最后一次的结果变成并行算法,对性能并无提高
        //因此下面的代码不会被执行
        //C0 = M1 + M4 - M5 + M7
        Parallel_Matrix_Evaluate(C0, n_2, n_2, cs, M1, n_2);
        Parallel_Matrix_AddSelf(C0, n_2, n_2, cs, M4, n_2);
        Parallel_Matrix_SubSelf(C0, n_2, n_2, cs, M5, n_2);
        Parallel_Matrix_AddSelf(C0, n_2, n_2, cs, M7, n_2);

        //C1 = M3 + M5
        Parallel_Matrix_Evaluate(C1, n_2, n_2, cs, M3, n_2);
        Parallel_Matrix_AddSelf(C1, n_2, n_2, cs, M5, n_2);

        //C2 = M2 + M4
        Parallel_Matrix_Evaluate(C2, n_2, n_2, cs, M2, n_2);
        Parallel_Matrix_AddSelf(C2, n_2, n_2, cs, M4, n_2);

        //C3 = M1 - M2 + M3 + M6
        Parallel_Matrix_Evaluate(C3, n_2, n_2, cs, M1, n_2);
        Parallel_Matrix_SubSelf(C3, n_2, n_2, cs, M2, n_2);
        Parallel_Matrix_AddSelf(C3, n_2, n_2, cs, M3, n_2);
        Parallel_Matrix_AddSelf(C3, n_2, n_2, cs, M6, n_2);
    }
    delete [] T1;
    delete [] T2;
    delete [] T3;
    delete [] T4;
    delete [] T5;
    delete [] T6;
    delete [] T7;
    delete [] T8;
    delete [] T9;
    delete [] T10;

    delete [] M1;
    delete [] M2;
    delete [] M3;
    delete [] M4;
    delete [] M5;
    delete [] M6;
    delete [] M7;

    int i;
    for ( i = 0; i < task_array.GetTaskCount(); i++ )
    {
        delete (CStrassenParam<T> *)(task_array.m_pTaskArray[i].pArg);
    }

    int col_a01 = col_a - n;
    int col_b01 = col_b - n;
    int row_a10 = row_a - n;
    int row_b10 = col_a01;

    T *H1 = NULL;
    T *H2 = NULL;
    T *H3 = NULL;
    T *H4 = NULL;

    CStrassenParam<T> *pSPara;
    CTaskArray  task_array1(7);


    // 上面己经计算出了C00 = A00*B00
    // 下面再计算C00 = A00 * B00 + A01 * B10
    if ( col_a01 > 0 )
    {
        // 计算H1 = A01 * B10
        H1 = new T[n * n];

        pSPara = new CStrassenParam<T>;
        pSPara->SetPara(A01, n, col_a01, as, 
            B10, col_a01, n, bs, H1, n, depth+1, max_depth, ss, pSched);
        task_array1.AddTask(Strassen_Matrix_Task<T>, pSPara);
    }

    // 计算C01 = A00 * B01 + A01 * B11
    if ( col_b01 > 0 )
    {
        pSPara = new CStrassenParam<T>;
        pSPara->SetPara(A00, n, n, as, 
            B01, n, col_b01, bs, C01, cs, depth+1, max_depth, ss, pSched);
        task_array1.AddTask(Strassen_Matrix_Task<T>, pSPara);

        if ( col_a01 > 0 )
        {
            H2 = new T[n * col_b01];

            pSPara = new CStrassenParam<T>;
            pSPara->SetPara(A01, n, col_a01, as, 
                B11, row_b10, col_b01, bs, 
                H2, col_b01, depth+1, max_depth, ss, pSched);
            task_array1.AddTask(Strassen_Matrix_Task<T>, pSPara);
        }
    }

    // 计算C10 = A10 * B00 + A11 * B10
    if ( row_a10 > 0 )
    {
        // 计算C10 = A10 * B00
        pSPara = new CStrassenParam<T>;
        pSPara->SetPara(A10, row_a10, n, as, B00, n, n, bs, 
            C10, cs, depth+1, max_depth, ss, pSched);
        task_array1.AddTask(Strassen_Matrix_Task<T>, pSPara);

        // C10 += A11 * B10
        if ( col_a01 > 0 )
        {
            // H3 = A11 * B10 
            H3 = new T[row_a10 * n];

            pSPara = new CStrassenParam<T>;
            pSPara->SetPara(A11, row_a10, col_a01, as, 
                B10, row_b10, n, bs, 
                H3, n, depth+1, max_depth, ss, pSched);
            task_array1.AddTask(Strassen_Matrix_Task<T>, pSPara);
        }
    }

    // 计算 C11 = A10 * B01 + A11 * B11
    if ( col_b01 > 0 )
    {
        if ( row_a10 > 0 )
        {
            pSPara = new CStrassenParam<T>;
            pSPara->SetPara(A10, row_a10, n, as, 
                B01, n, col_b01, bs, 
                C11, cs, depth+1, max_depth, ss, pSched);
            task_array1.AddTask(Strassen_Matrix_Task<T>, pSPara);

            if ( col_a01 > 0 )
            {
                H4 = new T[row_a10 * col_b01];

                pSPara = new CStrassenParam<T>;
                pSPara->SetPara(A11, row_a10, col_a01, as, 
                    B11, row_b10, col_b01, bs, 
                    H4, col_b01, depth+1, max_depth, ss, pSched);
                task_array1.AddTask(Strassen_Matrix_Task<T>, pSPara);
            }
        }
    }

    if ( task_array1.GetTaskCount() > 0 )
    {
        pSched->SpawnTaskArrayAndWait(&task_array1);
    }

    if ( H1 != NULL )
    {
        // C00 += H1
        Matrix_AddSelf(C00, n, n, cs, H1, n);
        delete [] H1;
    }

    if ( H2 != NULL )
    {
        // C01 += H2 (C01 = A00 * B01 + A01 * B11, H2 = A01 *B11)
        Matrix_AddSelf(C01, n, col_b01, cs, H2, col_b01);
        delete []  H2;
    }

    if ( H3 != NULL )
    {
        // C10 += H3
        Matrix_AddSelf(C10, row_a10, n, cs, H3, n);

        delete[] H3;
    }

    if ( H4 != NULL )
    {
        //C11 += H4;
        Matrix_AddSelf(C11, row_a10, col_b01, cs, H4, col_b01);
        delete [] H4;
    }

    for ( i = 0; i < task_array1.GetTaskCount(); i++ )
    {
        delete (CStrassenParam<T> *)(task_array1.m_pTaskArray[i].pArg);
    }

    return CAPI_SUCCESS;
}



/*  并行Strassen Matrix 乘法，使用任务调度进行并行计算
    可以对任意大小的矩阵进行Strassen乘法

    @param	T *a - 要相乘的矩阵	
    @param	int row_a - a要相乘的行数	
    @param	int col_a - a要相乘的列数	
    @param	int as - a的原始矩阵的一行大小（列数）	
    @param	T *b - 要相乘的矩阵	
    @param	int row_b - 矩阵b要相乘的行数	
    @param	int col_b - 矩阵b要相乘的列数	
    @param	int bs - 矩阵b的原始矩阵的一行大小（列数）	
    @param	T *c - 存放相乘结果的矩阵	
    @param	int cs - 矩阵c的原始矩阵的一行大小（列数）	
    @param	int ss - 最小的并行Strassen矩阵大小，
                     矩阵大小小于ss时将调用串行Strassen乘法函数	
    @param	int max_depth = DEF_STRASSEN_DEPTH - 最大递归深度	
    @return	void - 无	
*/
template <class T>
void Parallel_Strassen_Matrix_Multiply(T *a,  int row_a, int col_a, int as, 
                                       T *b,  int row_b, int col_b, int bs, 
                                       T *c, int cs,  
                                       int ss = MIN_PARALLEL_STRASSEN_MATRIX_SIZE, 
                                       int max_depth = DEF_STRASSEN_DEPTH)
{
    CWaitTaskScheduler *pSched = new CWaitTaskScheduler;

    CStrassenParam<T> *pPara = new CStrassenParam<T>;

    pPara->SetPara(a, row_a, col_a, as, b, row_b, col_b, bs, 
        c, cs, 0, max_depth, ss, pSched);

    CWaitTask task;
    task.func = Strassen_Matrix_Task<T>;
    task.pArg = pPara;

    pSched->BeginRootThread(task);

    delete pSched;

    return;
}


#if 0
/*  Strassen Matrix 乘法
    C = A * B
    将A,B,C分解成4个大小相等的矩阵如下：
    A = { A0  A1}     B = {B0  B1}    C = {C0  C1}
        { A2  A3}         {B2  B3}        {C2  C3}

    然后按以下计算过程计算
    M1 = (A0 + A3) * (B0 + B3)
    M2 = (A2 + A3) * B0
    M3 = A0 * (B1 - B3)
    M4 = A3 * (B2 - B0)
    M5 = (A0 + A1) * B3
    M6 = (A2 - A0) * (B0 + B1)
    M7 = (A1 - A3) * (B2 + B3)

    C0 = M1 + M4 - M5 + M7
    C1 = M3 + M5
    C2 = M2 + M4
    C3 = M1 - M2 + M3 + M6

    计算出C0 C1 C2 C3后，就得到了矩阵C的值
*/
template <class T>
void Serial_Strassen_Matrix_Multiply(T *a,  int row_a, int col_a, int as, 
                               T *b,  int row_b, int col_b, int bs, 
                               T *c, int cs, int depth, int max_depth = DEF_STRASSEN_DEPTH)
{
    int n;

    n = row_a;

    if ( n > row_b )
    {
        n = row_b;
    }
    if ( n > col_b )
    {
        n = col_b;
    }

    //将n转成偶数
    if ( (n & 0x1) == 1 )
    {
        n -= 1;
    }

    if ( n <= MIN_STRASSEN_MATRIX_SIZE || depth >= max_depth)
    {
        Matrix_Multiply2(a, row_a, col_a, as, b, row_b, col_b, bs, 
            c, cs); 
        return;
    }


    T *A00, *A01, *A10, *A11;
    T *B00, *B01, *B10, *B11;
    T *C00, *C01, *C10, *C11;

    A00 = a;
    A01 = a + n;
    A10 = a + n * as;
    A11 = a + n * as + n;

    B00 = b;
    B01 = b + n;
    B10 = b + n * bs;
    B11 = b + n * bs + n;

    C00 = c;
    C01 = c + n;
    C10 = c + n * cs;
    C11 = c + n * cs + n;

    int n_2 = n >> 1;

    T * T1 = new T[n_2 * n_2];
    T * T2 = new T[n_2 * n_2];

    T * M1 = new T[n_2 * n_2];
    T * M2 = new T[n_2 * n_2];
    T * M3 = new T[n_2 * n_2];
    T * M4 = new T[n_2 * n_2];
    T * M5 = new T[n_2 * n_2];
    T * M6 = new T[n_2 * n_2];
    T * M7 = new T[n_2 * n_2];


    T *A0, *A1, *A2, *A3;
    T *B0, *B1, *B2, *B3;
    T *C0, *C1, *C2, *C3;

    // A00 = {A0  A1}
    //       {A2  A3}
    A0 = a;
    A1 = a + n_2;
    A2 = a + n_2 * as;
    A3 = a + n_2 * as + n_2;

    // B00 = {B0  B1}
    //       {B2  B3}
    B0 = b;
    B1 = b + n_2;
    B2 = b + n_2 * bs;
    B3 = b + n_2 * bs + n_2;

    // C00 = {C0  C1}
    //       {C2  C3}
    C0 = c;
    C1 = c + n_2;
    C2 = c + n_2 * cs;
    C3 = c + n_2 * cs + n_2;

    //下面的代码计算A00*B00，将其拆分成7次矩阵相乘

    //T2=B1 - B3
    //M3=A0*T2 
    Matrix_Sub(B1, n_2, n_2, bs, B3, bs, T2, n_2);
    Serial_Strassen_Matrix_Multiply(A0, n_2, n_2, as, T2, n_2, n_2, n_2, 
        M3, n_2, depth+1, max_depth);

    //T1=A2 - A0 
    //T2=B0 + B1 
    //M6=T1*T2
    Matrix_Sub(A2, n_2, n_2, as, A0, as, T1, n_2);
    Matrix_Add(B0, n_2, n_2, bs, B1, bs, T2, n_2);
    Serial_Strassen_Matrix_Multiply(T1, n_2, n_2, n_2, T2, n_2, n_2, n_2, 
        M6, n_2, depth+1, max_depth);


    //T1=A2 + A3 
    //M2=T1*B0 
    Matrix_Add(A2, n_2, n_2, as, A3, as, T1, n_2);
    Serial_Strassen_Matrix_Multiply(T1, n_2, n_2, n_2, B0, n_2, n_2, bs, 
        M2, n_2, depth+1, max_depth);

    //T1=A0 + A3 
    //T2=B0 + B3 
    //M1=T1*T2 
    Matrix_Add(A0, n_2, n_2, as, A3, as, T1, n_2);
    Matrix_Add(B0, n_2, n_2, bs, B3, bs, T2, n_2);
    Serial_Strassen_Matrix_Multiply(T1, n_2, n_2, n_2, T2, n_2, n_2, n_2, 
        M1, n_2, depth+1, max_depth);

    //T1=A0 + A1 
    //M5=T1*B3 
    Matrix_Add(A0, n_2, n_2, as, A1, as, T1, n_2);
    Serial_Strassen_Matrix_Multiply(T1, n_2, n_2, n_2, B3, n_2, n_2, bs, 
        M5, n_2, depth+1, max_depth);

    //T1=A1 - A3 
    //T2=B2 + B3 
    //M7=T1*T2 
    Matrix_Sub(A1, n_2, n_2, as, A3, as, T1, n_2);
    Matrix_Add(B2, n_2, n_2, bs, B3, bs, T2, n_2);
    Serial_Strassen_Matrix_Multiply(T1, n_2, n_2, n_2, T2, n_2, n_2, n_2, 
        M7, n_2, depth+1, max_depth);

    //T2=B2 - B0
    //M4=A3*T2 
    Matrix_Sub(B2, n_2, n_2, bs, B0, bs, T2, n_2);
    Serial_Strassen_Matrix_Multiply(A3, n_2, n_2, as, T2, n_2, n_2, n_2, 
        M4, n_2, depth+1, max_depth);

    //C0 = M1 + M4 - M5 + M7
    Matrix_Evaluate(C0, n_2, n_2, cs, M1, n_2);
    Matrix_AddSelf(C0, n_2, n_2, cs, M4, n_2);
    Matrix_SubSelf(C0, n_2, n_2, cs, M5, n_2);
    Matrix_AddSelf(C0, n_2, n_2, cs, M7, n_2);

    //C1 = M3 + M5
    Matrix_Evaluate(C1, n_2, n_2, cs, M3, n_2);
    Matrix_AddSelf(C1, n_2, n_2, cs, M5, n_2);

    //C2 = M2 + M4
    Matrix_Evaluate(C2, n_2, n_2, cs, M2, n_2);
    Matrix_AddSelf(C2, n_2, n_2, cs, M4, n_2);

    //C3 = M1 - M2 + M3 + M6
    Matrix_Evaluate(C3, n_2, n_2, cs, M1, n_2);
    Matrix_SubSelf(C3, n_2, n_2, cs, M2, n_2);
    Matrix_AddSelf(C3, n_2, n_2, cs, M3, n_2);
    Matrix_AddSelf(C3, n_2, n_2, cs, M6, n_2);

    delete [] T1;
    delete [] T2;

    delete [] M1;
    delete [] M2;
    delete [] M3;
    delete [] M4;
    delete [] M5;
    delete [] M6;
    delete [] M7;

    int col_a01 = col_a - n;
    int col_b01 = col_b - n;
    int row_a10 = row_a - n;
    int row_b10 = col_a01;

    // 上面己经计算出了C00 = A00*B00
    // 下面再计算C00 = A00 * B00 + A01 * B10
    if ( col_a01 > 0 )
    {
        // 计算H = A01 * B10
        T * H = new T[n * n];

        Serial_Strassen_Matrix_Multiply(A01, n, col_a01, as, 
            B10, col_a01, n, bs, 
            H, n, depth+1, max_depth);

        // C00 += H
        Matrix_AddSelf(C00, n, n, cs, H, n);

        delete [] H;

    }

    // 计算C01 = A00 * B01 + A01 * B11
    if ( col_b01 > 0 )
    {
        Serial_Strassen_Matrix_Multiply(A00, n, n, as, 
            B01, n, col_b01, bs, 
            C01, cs, depth+1, max_depth);

        if ( col_a01 > 0 )
        {
            T *H = new T[n * col_b01];

            Serial_Strassen_Matrix_Multiply(A01, n, col_a01, as, 
                B11, row_b10, col_b01, bs, 
                H, col_b01, depth+1, max_depth);

            Matrix_AddSelf(C01, n, col_b01, cs, H, col_b01);
            delete []  H;
        }
    }


    // 计算C10 = A10 * B00 + A11 * B10
    if ( row_a10 > 0 )
    {
        // 计算C10 = A10 * B00
        Serial_Strassen_Matrix_Multiply(A10, row_a10, n, as, 
            B00, n, n, bs, 
            C10, cs, depth+1, max_depth);

        // C10 += A11 * B10
        if ( col_a01 > 0 )
        {
            // H = A11 * B10 
            T * H = new T[row_a10 * n];

            Serial_Strassen_Matrix_Multiply(A11, row_a10, col_a01, as, 
                B10, row_b10, n, bs, 
                H, n, depth+1, max_depth);

            // C10 += H
            Matrix_AddSelf(C10, row_a10, n, cs, H, n);

            delete[] H;

        }
    }

    // 计算 C11 = A10 * B01 + A11 * B11
    if ( col_b01 > 0 )
    {
        if ( row_a10 > 0 )
        {
            Serial_Strassen_Matrix_Multiply(A10, row_a10, n, as, 
                B01, n, col_b01, bs, 
                C11, cs, depth+1, max_depth);
            if ( col_a01 > 0 )
            {
                T *H = new T[row_a10 * col_b01];

                Serial_Strassen_Matrix_Multiply(A11, row_a10, col_a01, as, 
                    B11, row_b10, col_b01, bs, 
                    H, col_b01, depth+1, max_depth);

                Matrix_AddSelf(C11, row_a10, col_b01, cs, H, col_b01);

                delete [] H;
            }
        }
    }
    return;
}

#endif

#endif //__PARALLEL_STRASSEN_MATRIX_H__
