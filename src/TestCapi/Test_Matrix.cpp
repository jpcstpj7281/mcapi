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
#include <stdlib.h>
#include <stdio.h>
#include "CapiGlobal.h"
#include "TestApi.h"
#include "TestCapi.h"
#include <time.h>

#include <omp.h>
#include "Parallel_Matrix.h"
#include "Parallel_Strassen_Matrix.h"


void TestCase_Matrix_Multiply_1(void);
void TestCase_Matrix_Multiply_2(void);
void TestCase_Matrix_Multiply_3(void);
void TestCase_Matrix_Multiply_4(void);

void TestCase_Parallel_Matrixply_1(void);
void TestCase_Parallel_Matrixply_2(void);
void TestCase_Parallel_Matrixply_3(void);


void TestCase_Blocked_Matrix_Multiply(void);
void TestCase_Strassen_Matrix_Multiply(void);

void TestCase_Parallel_Blocked_Matrix_Multiply(void);
void TestCase_Parallel_Strassen_Matrix_Multiply(void);


void Test_Matrix()
{
    TestCase_Add(TestCase_Matrix_Multiply_1);
    TestCase_Add(TestCase_Matrix_Multiply_2);
    TestCase_Add(TestCase_Matrix_Multiply_3);
    TestCase_Add(TestCase_Matrix_Multiply_4);

    TestCase_Add(TestCase_Parallel_Matrixply_1);
    TestCase_Add(TestCase_Parallel_Matrixply_2);
    TestCase_Add(TestCase_Parallel_Matrixply_3);

    TestCase_Add(TestCase_Blocked_Matrix_Multiply);
    TestCase_Add(TestCase_Strassen_Matrix_Multiply);
    TestCase_Add(TestCase_Parallel_Blocked_Matrix_Multiply);
    TestCase_Add(TestCase_Parallel_Strassen_Matrix_Multiply);
}

REGISTER_TESTFUNC(Test_Matrix)


int a1[2][2] = {{11, 20}, {15, 30} };
int b1[2][2] = {{5, 8}, {50, 10}};
int r1[2][2] = {1055, 288, 1575, 420};

int x[2][3] = {{1,2,3}, {4,5,6}};
int y[3][2] = {{1,2}, {3,4},{5,6}};
int r[2][2] = {22, 28, 49, 64};

int a2[4][4] = {{5, 10, 15, 20}, 
                {100, 100, 150, 200}, 
                {30, 40, 50, 60}, 
                {70,80, 90, 100}};
int b2[4][4] = {{15,20, 25, 30}, 
                {200, 300, 250, 400}, 
                {15, 25, 35, 45}, 
                {170,180, 190, 200}};
int r2[4][4] = {{5700, 7075, 6950, 8825}, 
                {57750, 71750, 70750, 89750}, 
                {19400, 24650, 23900, 31150},
                {35400, 45650, 43900, 58150}};


int a3[3][3] = {1,2,3,4,5,6,7,8,9};
int b3[3][3] = {1,2,3,0,3,5,2,0,5};
int r3[3][3] = {7, 8, 28, 16, 23, 67, 25, 38, 106};



#if 0
TestCase_Matrix_Multiply_1
a[0][0] = 1055    a[0][1] = 288
a[1][0] = 1575    a[1][1] = 420
TestCase_Matrix_Multiply_2
a[0][0] = 5700    a[0][1] = 7075    a[0][2] = 6950    a[0][3] = 8825
a[1][0] = 57750    a[1][1] = 71750    a[1][2] = 70750    a[1][3] = 89750
a[2][0] = 19400    a[2][1] = 24650    a[2][2] = 23900    a[2][3] = 31150
a[3][0] = 35400    a[3][1] = 45650    a[3][2] = 43900    a[3][3] = 58150
TestCase_Matrix_Multiply_3
a[0][0] = 22    a[0][1] = 28
a[1][0] = 49    a[1][1] = 64
TestCase_Matrix_Multiply_4
a[0][0] = 7    a[0][1] = 8    a[0][2] = 28
a[1][0] = 16    a[1][1] = 23    a[1][2] = 67
a[2][0] = 25    a[2][1] = 38    a[2][2] = 106
TestCase_Parallel_Matrix_Multiply_1
a[0][0] = 1055    a[0][1] = 288
a[1][0] = 1575    a[1][1] = 420
TestCase_Parallel_Matrix_Multiply_2
a[0][0] = 5700    a[0][1] = 7075    a[0][2] = 6950    a[0][3] = 8825
a[1][0] = 57750    a[1][1] = 71750    a[1][2] = 70750    a[1][3] = 89750
a[2][0] = 19400    a[2][1] = 24650    a[2][2] = 23900    a[2][3] = 31150
a[3][0] = 35400    a[3][1] = 45650    a[3][2] = 43900    a[3][3] = 58150
TestCase_Parallel_Matrix_Multiply_3
a[0][0] = 22    a[0][1] = 28
a[1][0] = 49    a[1][1] = 64

#endif


void DumpMatrix(int *a, int row, int col)
{
    int i, j;

    for ( i = 0; i < row; i++ )
    {
        for ( j = 0; j < col; j++ )
        {
            printf( "a[%ld][%ld] = %ld    ", i, j, a[i*row+j] );
        }
        printf("\n");
    }
}


void TestCase_Matrix_Multiply_1(void)
{
    int c1[2][2];
    int c2[2][2];

    Matrix_Multiply(reinterpret_cast<int *>(a1), 2, 2, 2,
        reinterpret_cast<int *>(b1), 2, 2, 2, reinterpret_cast<int *>(c1), 2);

    Matrix_Compare(reinterpret_cast<int *>(c1), 2, 2, 2, reinterpret_cast<int *>(r1), 2);

    Matrix_Multiply2(reinterpret_cast<int *>(a1), 2, 2, 2,
        reinterpret_cast<int *>(b1), 2, 2, 2, reinterpret_cast<int *>(c2), 2);
    Matrix_Compare(reinterpret_cast<int *>(c2), 2, 2, 2, reinterpret_cast<int *>(r1), 2);

}

void TestCase_Matrix_Multiply_2(void)
{
    int c1[4][4];
    int c2[4][4];

    Matrix_Multiply(reinterpret_cast<int *>(a2), 4, 4, 4, 
        reinterpret_cast<int *>(b2), 4, 4, 4, reinterpret_cast<int *>(c1), 4);

    Matrix_Compare(reinterpret_cast<int *>(c1), 4, 4, 4, reinterpret_cast<int *>(r2), 4);

    Matrix_Multiply2(reinterpret_cast<int *>(a2), 4, 4, 4, 
        reinterpret_cast<int *>(b2), 4, 4, 4, reinterpret_cast<int *>(c2), 4);

    Matrix_Compare(reinterpret_cast<int *>(c2), 4, 4, 4, reinterpret_cast<int *>(r2), 4);
}


void TestCase_Matrix_Multiply_3(void)
{
    int c1[2][2];
    int c2[2][2];

    Matrix_Multiply(reinterpret_cast<int *>(x), 2, 3, 3,
        reinterpret_cast<int *>(y), 3, 2, 2, reinterpret_cast<int *>(c1), 2);

    Matrix_Compare(reinterpret_cast<int *>(c1), 2, 2, 2, reinterpret_cast<int *>(r), 2);

    Matrix_Multiply2(reinterpret_cast<int *>(x), 2, 3, 3,
        reinterpret_cast<int *>(y), 3, 2, 2, reinterpret_cast<int *>(c2), 2);

    Matrix_Compare(reinterpret_cast<int *>(c2), 2, 2, 2, reinterpret_cast<int *>(r), 2);

}

void TestCase_Matrix_Multiply_4(void)
{
    int c1[3][3];
    int c2[3][3];

    Matrix_Multiply(reinterpret_cast<int *>(a3), 3, 3, 3,
        reinterpret_cast<int *>(b3), 3, 3, 3, reinterpret_cast<int *>(c1), 3);

    Matrix_Compare(reinterpret_cast<int *>(c1), 3, 3, 3, reinterpret_cast<int *>(r3), 3);

    Matrix_Multiply2(reinterpret_cast<int *>(a3), 3, 3, 3,
        reinterpret_cast<int *>(b3), 3, 3, 3, reinterpret_cast<int *>(c2), 3);

    Matrix_Compare(reinterpret_cast<int *>(c2), 3, 3, 3, reinterpret_cast<int *>(r3), 3);

}


void TestCase_Parallel_Matrixply_1(void)
{
    int c1[2][2];

    Parallel_Matrix_Multiply(reinterpret_cast<int *>(a1), 2, 2, 2,
        reinterpret_cast<int *>(b1), 2, 2, 2, reinterpret_cast<int *>(c1), 2);

    Matrix_Compare(reinterpret_cast<int *>(c1), 2, 2, 2, reinterpret_cast<int *>(r1), 2);
}

void TestCase_Parallel_Matrixply_2(void)
{
    int c1[4][4];

    Parallel_Matrix_Multiply(reinterpret_cast<int *>(a2), 4, 4, 4,
        reinterpret_cast<int *>(b2), 4, 4, 4, reinterpret_cast<int *>(c1), 4);

    Matrix_Compare(reinterpret_cast<int *>(c1), 4, 4, 4, reinterpret_cast<int *>(r2), 4);
}

void TestCase_Parallel_Matrixply_3(void)
{
    int c1[2][2];

    Parallel_Matrix_Multiply(reinterpret_cast<int *>(x), 2, 3, 3,
        reinterpret_cast<int *>(y), 3, 2, 2, reinterpret_cast<int *>(c1), 2);

    Matrix_Compare(reinterpret_cast<int *>(c1), 2, 2, 2, reinterpret_cast<int *>(r), 2);
}


void TestCase_Blocked_Matrix_Multiply(void)
{
    int i;
    const int  SIZE = 256;

    int size = SIZE * SIZE;
    double *A  = new double [size];
    double *B  = new double [size];
    double *C  = new double [size];
    double *D =  new double [size];

    srand(time(NULL));
    for ( i = 0; i < size; i++ )
    {
        A[i] = (double)(rand() % SIZE);
        B[i] = (double)(rand() % SIZE);
    }


    clock_t t1, t2;

    t1 = clock();
    Matrix_Multiply(A, SIZE, SIZE, SIZE, B, SIZE, SIZE, SIZE, C, SIZE);
    t2 = clock();

    printf("\ntime1 = %ld\n", t2-t1);

    t1 = clock();
    for ( i = 0; i < 1; i++ )
    {
        Parallel_Matrix_Multiply(A, SIZE, SIZE, SIZE, B, SIZE, SIZE, SIZE, D, SIZE);
    }
    t2 = clock();

    printf("\nParallel_Matrix_Multiply(): %d * %d time = %ld ms\n", SIZE, SIZE, t2-t1);

    Matrix_Compare(C, SIZE, SIZE, SIZE, D, SIZE);

    t1 = clock();
    for ( i = 0; i < 1; i++ )
    {
        Matrix_Multiply2(A, SIZE, SIZE, SIZE, B, SIZE, SIZE, SIZE, D, SIZE);
    }
    t2 = clock();

    printf("\nMatrix_Multiply2(): %d * %d time = %ld ms\n", SIZE, SIZE, t2-t1);
    Matrix_Compare(C, SIZE, SIZE, SIZE, D, SIZE);

    t1 = clock();
    for ( i = 0; i < 1; i++ )
    {
        Blocked_Matrix_Multiply(A, SIZE, SIZE, SIZE, B, SIZE, SIZE,SIZE, D, SIZE, 64);
    }
    t2 = clock();

    printf("\nBlocked_Matrix_Multiply():%d * %d time = %ld ms\n", SIZE, SIZE, t2-t1);

    Matrix_Compare(C, SIZE, SIZE, SIZE, D, SIZE);


    delete [] A;
    delete [] B;
    delete [] C;
    delete [] D;
    return;
}

void TestCase_Strassen_Matrix_Multiply(void)
{
    int i;
    const int  SIZE = 263;

    int size = SIZE * SIZE;
    double *A  = new double [size];
    double *B  = new double [size];
    double *C  = new double [size];
    double *D =  new double [size];

    srand(time(NULL));
    for ( i = 0; i < size; i++ )
    {
        A[i] = (double)(rand() % SIZE);
        B[i] = (double)(rand() % SIZE);
    }

    Blocked_Matrix_Multiply(A, SIZE, SIZE, SIZE, B, SIZE, SIZE,SIZE, C, SIZE, 64);

    clock_t t1, t2;

    t1 = clock();
    Strassen_Matrix_Multiply(A, SIZE, SIZE, SIZE, B, SIZE, SIZE, SIZE, D, SIZE, 0);
    t2 = clock();
    printf("\nStrassen_Matrix_Multiply():%d * %d time = %ld ms\n", SIZE, SIZE, t2-t1);

    Matrix_Compare(C, SIZE, SIZE, SIZE, D, SIZE);

    delete [] A;
    delete [] B;
    delete [] C;
    delete [] D;
    return;
}



void TestCase_Parallel_Blocked_Matrix_Multiply(void)
{
    int i;
    const int  SIZE = 1024;

    int size = SIZE * SIZE;
    double *A  = new double [size];
    double *B  = new double [size];
    double *C  = new double [size];
    double *D =  new double [size];

    srand(time(NULL));
    for ( i = 0; i < size; i++ )
    {
        A[i] = (double)(rand() % SIZE);
        B[i] = (double)(rand() % SIZE);
    }


    clock_t t1, t2;

    t1 = clock();
    Blocked_Matrix_Multiply(A, SIZE, SIZE, SIZE, B, SIZE, SIZE,SIZE, C, SIZE, 64);
    t2 = clock();
    printf("\nBlocked_Matrix_Multiply():%d * %d time = %ld ms\n", SIZE, SIZE, t2-t1);

    t1 = clock();
    Parallel_Blocked_Matrix_Multiply(A, SIZE, SIZE, SIZE, B, SIZE, SIZE, SIZE, D, SIZE, 32);
    t2 = clock();
    printf("\nParallel_Blocked_Matrix_Multiply():%d * %d time = %ld ms\n", SIZE, SIZE, t2-t1);

    Matrix_Compare(C, SIZE, SIZE, SIZE, D, SIZE);

    delete [] A;
    delete [] B;
    delete [] C;
    delete [] D;
    return;
}

void TestCase_Parallel_Strassen_Matrix_Multiply(void)
{
    int i;
#define TEST_SIZE   18

    const int XROWS =  2048;
    const int XCOLS =  2048;
    const int YROWS =  2048;
    const int YCOLS =  2048;

    int *x = new int[XROWS * XCOLS];
    int *y = new int[YROWS * YCOLS];
    int *z = new int[XROWS * YCOLS];
    int *r = new int[XROWS * YCOLS];

    for ( i = 0; i < XROWS * XCOLS; i++ )
    {
        x[i] = rand() % TEST_SIZE;
    }
    for ( i = 0; i < YROWS * YCOLS; i++ )
    {
        y[i] = rand() % TEST_SIZE;
    }

    clock_t t1, t2;


    t1 = clock();

    Blocked_Matrix_Multiply(x, XROWS, XCOLS, XCOLS, y, YROWS, YCOLS, YCOLS,
        r, YCOLS, DEF_MATRIX_BLOCK_SIZE);

    t2 = clock();

    printf("Blocked_Matrix_Multiply %d * %d  time = %ld ms\n", XROWS, YCOLS, t2-t1);

    t1 = clock();

    Matrix_Multiply2(x, XROWS, XCOLS, XCOLS, y, YROWS, YCOLS, YCOLS,
        z, YCOLS);

    t2 = clock();

    printf("Matrix_Multiply2 %d * %d  time = %ld ms\n", XROWS, YCOLS, t2-t1);

    Matrix_Compare(z, XROWS, YCOLS, YCOLS, r, YCOLS);

    t1 = clock();

    Parallel_Blocked_Matrix_Multiply(x, XROWS, XCOLS, XCOLS, y, YROWS, YCOLS, YCOLS,
        r, YCOLS, DEF_MATRIX_BLOCK_SIZE);

    t2 = clock();

    printf("Parallel_Blocked_Matrix_Multiply %d * %d  time = %ld ms\n", XROWS, YCOLS, t2-t1);

    Matrix_Compare(z, XROWS, YCOLS, YCOLS, r, YCOLS);

#if 0
    printf("Matrix x:\n");
    Matrix_Print2(x, XROWS, XCOLS, XCOLS);

    printf("Matrix y:\n");
    Matrix_Print2(y, YROWS, YCOLS, YCOLS);

    printf("Matrix Z:\n");
    Matrix_Print2(z, XROWS, YCOLS, YCOLS);
#endif

    t1 = clock();
    Strassen_Matrix_Multiply(x, XROWS, XCOLS, XCOLS, y, YROWS, YCOLS, YCOLS, 
        r, YCOLS, 0);
    t2 = clock();

    printf("Serial_Strassen Matrix %d * %d  time = %ld ms\n", XROWS, YCOLS, t2-t1);

    Matrix_Compare(z, XROWS, YCOLS, YCOLS, r, YCOLS);


#if 1
    t1 = clock();
    Parallel_Strassen_Matrix_Multiply<int>(x, XROWS, XCOLS, XCOLS, y, YROWS, YCOLS, YCOLS, 
        r, YCOLS, 256);
    t2 = clock();

    printf("Parallel_Strassen Matrix %d * %d  time = %ld ms\n", XROWS, YCOLS, t2-t1);

    Matrix_Compare(z, XROWS, YCOLS, YCOLS, r, YCOLS);
#endif
#if 0
    printf("Matrix R:\n");
    Matrix_Print2(r, XROWS, YCOLS, YCOLS);
#endif

    delete [] x;
    delete [] y;
    delete [] z;
    delete [] r;

    return;
}

