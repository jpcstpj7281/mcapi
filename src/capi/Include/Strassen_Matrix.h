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
 *	Strassen_Matrix.h
 *
 *	DESCRIPTION
 *		Interface to a Strassen Matrix Mulitplication operation.
 *
 *	HISTORY
 *      05-29-2009  create by zhouweiming. 
 *
 */

#ifndef __STRASSEN_MATRIX_H__
#define __STRASSEN_MATRIX_H__


#define MIN_STRASSEN_MATRIX_SIZE    64

#define DEF_STRASSEN_DEPTH          7

/*  ���� Strassen Matrix �˷�
    ���Զ������С�ľ������Strassen����˷�

    C = A * B
    �Ƚ�A��B��C��ֳ�������ʽ��
    A = { A00  A01}     B = {B00  B01}    C = {C00  C01}
        { A10  A11}         {B10  B11}        {C10  C11}

    ����A00,B00,C00����n��n��С�ľ���nΪż����
    ��������Cʱ�����·������
    C00 = A00 * B00 + A01 * B10
    C01 = A00 * B01 + A01 * B11
    C10 = A10 * B00 + A11 * B10
    C11 = A10 * B01 + A11 * B11

    �ܹ���8������˷��������A00*B00����Strassen������A00,B00,C00
    �ֽ��4����С��ȵľ������£�
    A00 = { A0  A1}     B00 = {B0  B1}    C00 = {C0  C1}
          { A2  A3}         {B2  B3}        {C2  C3}

    Ȼ�����¼�����̼���
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

    �����C0 C1 C2 C3�󣬾͵õ���A00*B00��ֵ,Ȼ��A01*B10, A00*B01������7��
    ����˷�ʹ�õݹ����Strassen�˷��Ϳ������
    ��������C00��C01��C10��C11������˾���C��A*B

	@param	T *a - Ҫ��˵ľ���	
	@param	int row_a - aҪ��˵�����	
	@param	int col_a - aҪ��˵�����	
	@param	int as - a��ԭʼ�����һ�д�С��������	
	@param	T *b - Ҫ��˵ľ���	
	@param	int row_b - ����bҪ��˵�����	
	@param	int col_b - ����bҪ��˵�����	
	@param	int bs - ����b��ԭʼ�����һ�д�С��������	
	@param	T *c - �����˽���ľ���	
	@param	int cs - ����c��ԭʼ�����һ�д�С��������	
	@param	int depth - �ڲ�ʹ�õĲ�������ʾ�ݹ���ȣ��������ʱ������0	
	@param	int max_depth = DEF_STRASSEN_DEPTH - ���ݹ����	
	@return	void - ��	
*/
template <class T>
void Strassen_Matrix_Multiply(T *a,  int row_a, int col_a, int as, 
                               T *b,  int row_b, int col_b, int bs, 
                               T *c, int cs, 
                               int depth, int max_depth = DEF_STRASSEN_DEPTH)
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

    //��nת��ż��
    if ( (n & 0x1) == 1 )
    {
        n -= 1;
    }

    if ( n <= MIN_STRASSEN_MATRIX_SIZE || depth >= max_depth)
    {
        Matrix_Multiply2(a, row_a, col_a, as, b, row_b, col_b, bs, c, cs); 
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

    //����Ĵ������A00*B00�������ֳ�7�ξ������

    //T2=B1 - B3
    //M3=A0*T2 
    //C1=M3,
    //C3=M3
    Matrix_Sub(B1, n_2, n_2, bs, B3, bs, T2, n_2);
    Strassen_Matrix_Multiply(A0, n_2, n_2, as, T2, n_2, n_2, n_2, 
        M3, n_2, depth+1, max_depth);
    Matrix_Evaluate(C1, n_2, n_2, cs, M3, n_2);
    Matrix_Evaluate(C3, n_2, n_2, cs, M3, n_2);

    //T1=A2 - A0 
    //T2=B0 + B1 
    //M6=T1*T2
    //C3=C3+M6
    Matrix_Sub(A2, n_2, n_2, as, A0, as, T1, n_2);
    Matrix_Add(B0, n_2, n_2, bs, B1, bs, T2, n_2);
    Strassen_Matrix_Multiply(T1, n_2, n_2, n_2, T2, n_2, n_2, n_2, 
        M6, n_2, depth+1, max_depth);
    Matrix_AddSelf(C3, n_2, n_2, cs, M6, n_2);


    //T1=A2 + A3
    //M2=T1*B0
    //C2=M2, 
    //C3=C3-M2
    Matrix_Add(A2, n_2, n_2, as, A3, as, T1, n_2);
    Strassen_Matrix_Multiply(T1, n_2, n_2, n_2, B0, n_2, n_2, bs, 
        M2, n_2, depth+1, max_depth);
    Matrix_Evaluate(C2, n_2, n_2, cs, M2, n_2);
    Matrix_SubSelf(C3, n_2, n_2, cs, M2, n_2);

    //T1=A0 + A3 
    //T2=B0 + B3 
    //M1=T1*T2 
    //C0=M1, 
    //C3=C3+M1
    Matrix_Add(A0, n_2, n_2, as, A3, as, T1, n_2);
    Matrix_Add(B0, n_2, n_2, bs, B3, bs, T2, n_2);
    Strassen_Matrix_Multiply(T1, n_2, n_2, n_2, T2, n_2, n_2, n_2, 
        M1, n_2, depth+1, max_depth);
    Matrix_Evaluate(C0, n_2, n_2, cs, M1, n_2);
    Matrix_AddSelf(C3, n_2, n_2, cs, M1, n_2);

    //T1=A0 + A1 
    //M5=T1*B3 
    //C0=C0-M5, 
    //C1=C1+M5
    Matrix_Add(A0, n_2, n_2, as, A1, as, T1, n_2);
    Strassen_Matrix_Multiply(T1, n_2, n_2, n_2, B3, n_2, n_2, bs, 
        M5, n_2, depth+1, max_depth);
    Matrix_SubSelf(C0, n_2, n_2, cs, M5, n_2);
    Matrix_AddSelf(C1, n_2, n_2, cs, M5, n_2);

    //T1=A1 - A3 
    //T2=B2 + B3 
    //M7=T1*T2 
    //C0=C0+M7
    Matrix_Sub(A1, n_2, n_2, as, A3, as, T1, n_2);
    Matrix_Add(B2, n_2, n_2, bs, B3, bs, T2, n_2);
    Strassen_Matrix_Multiply(T1, n_2, n_2, n_2, T2, n_2, n_2, n_2, 
        M7, n_2, depth+1, max_depth);
    Matrix_AddSelf(C0, n_2, n_2, cs, M7, n_2);

    //T2=B2 ? B0
    //M4=A3*T2
    //C0=C0+M4
    //C2=C2+M4
    Matrix_Sub(B2, n_2, n_2, bs, B0, bs, T2, n_2);
    Strassen_Matrix_Multiply(A3, n_2, n_2, as, T2, n_2, n_2, n_2, 
        M4, n_2, depth+1, max_depth);
    Matrix_AddSelf(C0, n_2, n_2, cs, M4, n_2);
    Matrix_AddSelf(C2, n_2, n_2, cs, M4, n_2);

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

    // ���漺���������C00 = A00*B00
    // �����ټ���C00 = A00 * B00 + A01 * B10
    if ( col_a01 > 0 )
    {
        // ����H = A01 * B10
        T * H = new T[n * n];

        Strassen_Matrix_Multiply(A01, n, col_a01, as, 
            B10, col_a01, n, bs, 
            H, n, depth+1, max_depth);

        // C00 += H
        Matrix_AddSelf(C00, n, n, cs, H, n);

        delete [] H;

    }

    // ����C01 = A00 * B01 + A01 * B11
    if ( col_b01 > 0 )
    {
        Strassen_Matrix_Multiply(A00, n, n, as, 
            B01, n, col_b01, bs, 
            C01, cs, depth+1, max_depth);

        if ( col_a01 > 0 )
        {
            T *H = new T[n * col_b01];

            Strassen_Matrix_Multiply(A01, n, col_a01, as, 
                B11, row_b10, col_b01, bs, 
                H, col_b01, depth+1, max_depth);

            Matrix_AddSelf(C01, n, col_b01, cs, H, col_b01);
            delete []  H;
        }
    }


    // ����C10 = A10 * B00 + A11 * B10
    if ( row_a10 > 0 )
    {
        // ����C10 = A10 * B00
        Strassen_Matrix_Multiply(A10, row_a10, n, as, 
            B00, n, n, bs, 
            C10, cs, depth+1, max_depth);

        // C10 += A11 * B10
        if ( col_a01 > 0 )
        {
            // H = A11 * B10 
            T * H = new T[row_a10 * n];

            Strassen_Matrix_Multiply(A11, row_a10, col_a01, as, 
                B10, row_b10, n, bs, 
                H, n, depth+1, max_depth);

            // C10 += H
            Matrix_AddSelf(C10, row_a10, n, cs, H, n);

            delete[] H;

        }
    }

    // ���� C11 = A10 * B01 + A11 * B11
    if ( col_b01 > 0 )
    {
        if ( row_a10 > 0 )
        {
            Strassen_Matrix_Multiply(A10, row_a10, n, as, 
                B01, n, col_b01, bs, 
                C11, cs, depth+1, max_depth);
            if ( col_a01 > 0 )
            {
                T *H = new T[row_a10 * col_b01];

                Strassen_Matrix_Multiply(A11, row_a10, col_a01, as, 
                    B11, row_b10, col_b01, bs, 
                    H, col_b01, depth+1, max_depth);

                Matrix_AddSelf(C11, row_a10, col_b01, cs, H, col_b01);

                delete [] H;
            }
        }
    }
    return;
}


/**	��׼��С��Strassen����˷�����
    ����ľ����Сn������2���ݵ�������
    ������������޷���ֳ�4����ͬ��С�ľ��������

    @param	T *a - Ҫ��˵ľ���	
    @param	int row_a - aҪ��˵�����	
    @param	int col_a - aҪ��˵�����	
    @param	int as - a��ԭʼ�����һ�д�С��������	
    @param	T *b - Ҫ��˵ľ���	
    @param	int row_b - ����bҪ��˵�����	
    @param	int col_b - ����bҪ��˵�����	
    @param	int bs - ����b��ԭʼ�����һ�д�С��������	
    @param	T *c - �����˽���ľ���	
    @param	int cs - ����c��ԭʼ�����һ�д�С��������	
    @param	int depth - �ڲ�ʹ�õĲ�������ʾ�ݹ���ȣ��������ʱ������0	
    @param	int max_depth = DEF_STRASSEN_DEPTH - ���ݹ����	
    @return	void - ��	
*/
template <class T>
void Strassen_Matrix_MultiplyNN(T *a, T *b, int n, int as, int bs,
                              T *c, int cs, 
                              int depth, int max_depth = DEF_STRASSEN_DEPTH)
{
    if ( n <= MIN_STRASSEN_MATRIX_SIZE || depth >= max_depth)
    {
        Matrix_MultiplyNN(a, b, n, as, bs, c, cs); 
        return;
    }

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

    A0 = a;
    A1 = a + n_2;
    A2 = a + n_2 * as;
    A3 = a + n_2 * as + n_2;

    B0 = b;
    B1 = b + n_2;
    B2 = b + n_2 * bs;
    B3 = b + n_2 * bs + n_2;

    C0 = c;
    C1 = c + n_2;
    C2 = c + n_2 * cs;
    C3 = c + n_2 * cs + n_2;


    //T2=B1 - B3 
    //M3=A0*T2 
    //C1=M3,
    //C3=M3
    Matrix_Sub(B1, n_2, n_2, bs, B3, bs, T2, n_2);
#if 0
    printf("Matrix B1:\n");
    Matrix_Print2(B1, n_2, n_2, bs);
    printf("Matrix B3:\n");
    Matrix_Print2(B3, n_2, n_2, bs);
#endif
    Strassen_Matrix_MultiplyNN(A0, T2, n_2, as, n_2, 
        M3, n_2, depth+1, max_depth);
#if 0
    printf("Matrix A0:\n");
    Matrix_Print2(A0, n_2, n_2, as);
    printf("Matrix T2:\n");
    Matrix_Print2(T2, n_2, n_2, n_2);

    printf("Matrix M3:\n");
    Matrix_Print2(M3, n_2, n_2, n_2);
#endif
    Matrix_Evaluate(C1, n_2, n_2, cs, M3, n_2);
    Matrix_Evaluate(C3, n_2, n_2, cs, M3, n_2);
#if 0
    printf("Matrix C1:\n");
    Matrix_Print2(C1, n_2, n_2, cs);
    printf("\n");
#endif
    //T1=A2 - A0 
    //T2=B0 + B1 
    //M6=T1*T2 
    //C3=C3+M6
    Matrix_Sub(A2, n_2, n_2, as, A0, as, T1, n_2);
    Matrix_Add(B0, n_2, n_2, bs, B1, bs, T2, n_2);
    Strassen_Matrix_MultiplyNN(T1, T2, n_2, n_2, n_2, 
        M6, n_2, depth+1, max_depth);
    Matrix_AddSelf(C3, n_2, n_2, cs, M6, n_2);


    //T1=A2 + A3 
    //M2=T1*B0 
    //C2=M2, 
    //C3=C3-M2
    Matrix_Add(A2, n_2, n_2, as, A3, as, T1, n_2);
    Strassen_Matrix_MultiplyNN(T1, B0, n_2, n_2, bs,
        M2, n_2, depth+1, max_depth);
    Matrix_Evaluate(C2, n_2, n_2, cs, M2, n_2);
    Matrix_SubSelf(C3, n_2, n_2, cs, M2, n_2);

    //T1=A0 + A3 
    //T2=B0 + B3 
    //M1=T1*T2 
    //C0=M1, 
    //C3=C3+M1
    Matrix_Add(A0, n_2, n_2, as, A3, as, T1, n_2);
    Matrix_Add(B0, n_2, n_2, bs, B3, bs, T2, n_2);
    Strassen_Matrix_MultiplyNN(T1, T2, n_2, n_2, n_2, 
        M1, n_2, depth+1, max_depth);
    Matrix_Evaluate(C0, n_2, n_2, cs, M1, n_2);
    Matrix_AddSelf(C3, n_2, n_2, cs, M1, n_2);

    //T1=A0 + A1 
    //M5=T1*B3 
    //C0=C0?M5, 
    //C1=C1+M5
    Matrix_Add(A0, n_2, n_2, as, A1, as, T1, n_2);
    Strassen_Matrix_MultiplyNN(T1, B3, n_2, n_2, bs, 
        M5, n_2, depth+1, max_depth);
    Matrix_SubSelf(C0, n_2, n_2, cs, M5, n_2);
    Matrix_AddSelf(C1, n_2, n_2, cs, M5, n_2);

    //T1=A1 - A3 
    //T2=B2 + B3 
    //M7=T1*T2 
    //C0=C0+M7
    Matrix_Sub(A1, n_2, n_2, as, A3, as, T1, n_2);
    Matrix_Add(B2, n_2, n_2, bs, B3, bs, T2, n_2);
    Strassen_Matrix_MultiplyNN(T1, T2, n_2, n_2, n_2, 
        M7, n_2, depth+1, max_depth);
    Matrix_AddSelf(C0, n_2, n_2, cs, M7, n_2);

    //T2=B2 - B0
    //M4=A3*T2
    //C0=C0+M4
    //C2=C2+M4
    Matrix_Sub(B2, n_2, n_2, bs, B0, bs, T2, n_2);
    Strassen_Matrix_MultiplyNN(A3, T2, n_2, as, n_2, 
        M4, n_2, depth+1, max_depth);
    Matrix_AddSelf(C0, n_2, n_2, cs, M4, n_2);
    Matrix_AddSelf(C2, n_2, n_2, cs, M4, n_2);

    delete [] T1;
    delete [] T2;

    delete [] M1;
    delete [] M2;
    delete [] M3;
    delete [] M4;
    delete [] M5;
    delete [] M6;
    delete [] M7;

    return;
}




#endif //__STRASSEN_MATRIX_H__