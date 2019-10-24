/*
name:       Kang Sinil
loginID:    sinil.gang
*/
/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 32)
    {
        int rowCount, columnCount, k;
        int container0, container1, container2, container3, 
            container4, container5, container6, container7;
        for (rowCount = 0; rowCount < N; rowCount += 8) 
        {
            for (columnCount = 0; columnCount < M; columnCount += 8) 
            {
                for (k = rowCount; k < rowCount + 8; k++) 
                {
                        container0 = A[k][columnCount];
                        container1 = A[k][columnCount + 1];
                        container2 = A[k][columnCount + 2];
                        container3 = A[k][columnCount + 3];
                        container4 = A[k][columnCount + 4];
                        container5 = A[k][columnCount + 5];
                        container6 = A[k][columnCount + 6];
                        container7 = A[k][columnCount + 7];

                        B[columnCount][k] = container0;
                        B[columnCount + 1][k] = container1;
                        B[columnCount + 2][k] = container2;
                        B[columnCount + 3][k] = container3;
                        B[columnCount + 4][k] = container4;
                        B[columnCount + 5][k] = container5;
                        B[columnCount + 6][k] = container6;
                        B[columnCount + 7][k] = container7;
                }
            }
        }
    }
    else if (M == 64)
    {        
      int rowCount, columnCount;
      int k, l;
      int container1, container2, container3, container4, container5, container6, container7, container8;
        for (rowCount = 0; rowCount < N; rowCount += 8) 
        {
            for (columnCount = 0; columnCount < M; columnCount += 8) 
            {
                for (k = rowCount; k < rowCount + 4; k++) 
                {
                    container1 = A[k][columnCount];
                    container2 = A[k][columnCount + 1];
                    container3 = A[k][columnCount + 2];
                    container4 = A[k][columnCount + 3];
                    container5 = A[k][columnCount + 4];
                    container6 = A[k][columnCount + 5];
                    container7 = A[k][columnCount + 6];
                    container8 = A[k][columnCount + 7];

                    // container 1~4 is stored directly,
                    B[columnCount][k] = container1;
                    B[columnCount + 1][k] = container2;
                    B[columnCount + 2][k] = container3;
                    B[columnCount + 3][k] = container4;
                    // container 5~8 is stored temporary.. sequetially, right to container 1~4
                    const int optimizedSpace = k + 4;
                    B[columnCount][optimizedSpace] = container5;
                    B[columnCount + 1][optimizedSpace] = container6;
                    B[columnCount + 2][optimizedSpace] = container7;
                    B[columnCount + 3][optimizedSpace] = container8;
                }   // After this loop, preparation is finished
                for (l = columnCount + 4; l < columnCount + 8; l++) 
                {
                    const int directSpace = l - 4;
                    // container 5~8 change to correct space directly.
                    container5 = A[rowCount + 4][directSpace];
                    container6 = A[rowCount + 5][directSpace];
                    container7 = A[rowCount + 6][directSpace];
                    container8 = A[rowCount + 7][directSpace];

                    // container 1~4 relocate at correct space, they was located right in upper loop
                    container1 = B[directSpace][rowCount + 4];
                    container2 = B[directSpace][rowCount + 5];
                    container3 = B[directSpace][rowCount + 6];
                    container4 = B[directSpace][rowCount + 7];

                    B[directSpace][rowCount + 4] = container5;
                    B[directSpace][rowCount + 5] = container6;
                    B[directSpace][rowCount + 6] = container7;
                    B[directSpace][rowCount + 7] = container8;

                    // Relocate it
                    B[l][rowCount] = container1;
                    B[l][rowCount + 1] = container2;
                    B[l][rowCount + 2] = container3;
                    B[l][rowCount + 3] = container4;

                    // Final 4x4 matrix is located directly.
                    B[l][rowCount + 4] = A[rowCount + 4][l];
                    B[l][rowCount + 5] = A[rowCount + 5][l];
                    B[l][rowCount + 6] = A[rowCount + 6][l];
                    B[l][rowCount + 7] = A[rowCount + 7][l];
                }   // After this loop, 8 x 8 matrix is completed
            }
        }
    }
    else if (M == 61)
    {
        int rowCount, columnCount;
        int k, l;
        int temp;
        int sizeIncreased = 23;
        for (rowCount = 0; rowCount < N; rowCount += sizeIncreased) 
        {
            for (columnCount = 0;  columnCount < M; columnCount += sizeIncreased) 
            {
                for (k = rowCount; (k < rowCount + sizeIncreased) && k < N; k++) 
                {
                    for (l = columnCount; (l < columnCount + sizeIncreased) && l < M; l++) 
                    {
                        temp = A[k][l];
                        B[l][k] = temp;
                    }
                }
            }
        }
    }

}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

