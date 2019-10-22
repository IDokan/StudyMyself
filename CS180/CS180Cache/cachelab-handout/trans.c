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
        for (rowCount = 0; rowCount < N; rowCount += 8) {
            for (columnCount = 0; columnCount < M; columnCount += 8) {
                for (k = rowCount; k < rowCount + 8; k++) {
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
      int i, j, k, l, t1, t2, t3, t4, t5, t6, t7, t8;
        for (i = 0; i < N; i += 8) {
            for (j = 0; j < M; j += 8) {
                for (k = i; k < i + 4; k++) {
                    t1 = A[k][j];
                    t2 = A[k][j + 1];
                    t3 = A[k][j + 2];
                    t4 = A[k][j + 3];
                    t5 = A[k][j + 4];
                    t6 = A[k][j + 5];
                    t7 = A[k][j + 6];
                    t8 = A[k][j + 7];

                    B[j][k] = t1;
                    B[j + 1][k] = t2;
                    B[j + 2][k] = t3;
                    B[j + 3][k] = t4;
                    B[j][k + 4] = t5;
                    B[j + 1][k + 4] = t6;
                    B[j + 2][k + 4] = t7;
                    B[j + 3][k + 4] = t8;
                }
                for (l = j + 4; l < j + 8; l++) {

                    t5 = A[i + 4][l - 4];
                    t6 = A[i + 5][l - 4];
                    t7 = A[i + 6][l - 4];
                    t8 = A[i + 7][l - 4];

                    t1 = B[l - 4][i + 4];
                    t2 = B[l - 4][i + 5];
                    t3 = B[l - 4][i + 6];
                    t4 = B[l - 4][i + 7];

                    B[l - 4][i + 4] = t5;
                    B[l - 4][i + 5] = t6;
                    B[l - 4][i + 6] = t7;
                    B[l - 4][i + 7] = t8;

                    B[l][i] = t1;
                    B[l][i + 1] = t2;
                    B[l][i + 2] = t3;
                    B[l][i + 3] = t4;

                    B[l][i + 4] = A[i + 4][l];
                    B[l][i + 5] = A[i + 5][l];
                    B[l][i + 6] = A[i + 6][l];
                    B[l][i + 7] = A[i + 7][l];
                }
            }
        }
    }
    else
    {
    // ************************************************
    int rowIndex, colIndex; // Indices to pass through rows/columns in blocks
    int blockedRowIndex, blockedColIndex; // Indices to step within blocks
    int temp;
    for(rowIndex = 0; rowIndex < N; rowIndex += 16) {
        for(colIndex = 0; colIndex < M; colIndex += 4) {
            // Do not try to transpose outside of the bounds of the matrix
            for(blockedRowIndex = rowIndex; (blockedRowIndex < rowIndex + 16) &&
                (blockedRowIndex < N); ++blockedRowIndex) {
                for(blockedColIndex = colIndex; (blockedColIndex < colIndex + 4)
                    && (blockedColIndex < M); ++blockedColIndex) {
                    // Store diagonals
                    if (blockedRowIndex - rowIndex == 
                        blockedColIndex - colIndex) {
                        temp = A[blockedRowIndex][blockedColIndex];
                    }
                    else { // Do a normal in-block swap
                        B[blockedColIndex][blockedRowIndex] = 
                            A[blockedRowIndex][blockedColIndex];
                    }
                }
                for (blockedColIndex = colIndex; (blockedColIndex < 
                    colIndex + 4) && (blockedColIndex < M); ++blockedColIndex) {
                    // Map the diagonals
                    if (blockedRowIndex - rowIndex == 
                        blockedColIndex - colIndex) {
                        B[blockedColIndex][blockedRowIndex] = temp;
                    }
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

