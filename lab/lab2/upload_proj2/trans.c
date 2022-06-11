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
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
  int i, j, tmp, index;
  int row_Block, col_Block;  // top row and column manupulation with stride
  if (M == 32) {  // divide the the 32X32 block into 8X8 , decrease the number
    // of misses
    // stride is 8, for 8X4=32, each
    for (row_Block = 0; row_Block < N; row_Block += 8) {
      for (col_Block = 0; col_Block < M; col_Block += 8) {
        for (i = row_Block; i < row_Block + 8; i++) {
          for (j = col_Block; j < col_Block + 8; j++) {
            if (i != j) {
              B[j][i] = A[i][j];  // tanspose
            } else {
              tmp = A[i][j];  // i==j means the data is on the diagonal. if we
                              // set B right now ,the  misses and
                              // evictions will increase . because
                              // the cache set of B is same to A.
              index = i;
            }
          }
          if (col_Block == row_Block) {  // just set B on the diagonal. other
            // than shouldn't set the B
            B[index][index] = tmp;
          }
        }
      }
    }
  }

  else if (M == 64) {
    // using 4 + 8 = 12 variable
    int t, t1, t2, t3, t4, t5, t6, t7, t8;
    for (row_Block = 0; row_Block < N; row_Block += 8)
      for (col_Block = 0; col_Block < M; col_Block += 8)  // blocking
      {
        // save and load
        for (t = row_Block; t < row_Block + 4; ++t) {
          // read all only once
          t1 = A[t][col_Block];
          t2 = A[t][col_Block + 1];
          t3 = A[t][col_Block + 2];
          t4 = A[t][col_Block + 3];
          t5 = A[t][col_Block + 4];
          t6 = A[t][col_Block + 5];
          t7 = A[t][col_Block + 6];
          t8 = A[t][col_Block + 7];

          B[col_Block][t] = t1;
          B[col_Block + 1][t] = t2;
          B[col_Block + 2][t] = t3;
          B[col_Block + 3][t] = t4;
          B[col_Block][t + 4] = t5;
          B[col_Block + 1][t + 4] = t6;
          B[col_Block + 2][t + 4] = t7;
          B[col_Block + 3][t + 4] = t8;
        }
        for (t = col_Block; t < col_Block + 4; ++t) {
          t1 = A[row_Block + 4][t];
          t2 = A[row_Block + 5][t];
          t3 = A[row_Block + 6][t];
          t4 = A[row_Block + 7][t];
          t5 = B[t][row_Block + 4];
          t6 = B[t][row_Block + 5];
          t7 = B[t][row_Block + 6];
          t8 = B[t][row_Block + 7];

          B[t][row_Block + 4] = t1;
          B[t][row_Block + 5] = t2;
          B[t][row_Block + 6] = t3;
          B[t][row_Block + 7] = t4;
          B[t + 4][row_Block] = t5;
          B[t + 4][row_Block + 1] = t6;
          B[t + 4][row_Block + 2] = t7;
          B[t + 4][row_Block + 3] = t8;
        }
        for (t = row_Block + 4; t < row_Block + 8; ++t) {
          t1 = A[t][col_Block + 4];
          t2 = A[t][col_Block + 5];
          t3 = A[t][col_Block + 6];
          t4 = A[t][col_Block + 7];
          B[col_Block + 4][t] = t1;
          B[col_Block + 5][t] = t2;
          B[col_Block + 6][t] = t3;
          B[col_Block + 7][t] = t4;
        }
      }
  }

  else {
    // separate the the 61X67 block into 16X16 , decrease the number of misses
    for (row_Block = 0; row_Block < N; row_Block += 16) {
      for (col_Block = 0; col_Block < M; col_Block += 16) {
        for (i = row_Block; i < row_Block + 16 && (i < N); i++) {
          for (j = col_Block; j < col_Block + 16 && (j < M); j++) {
            if (i != j) {
              B[j][i] = A[i][j];
            } else {
              tmp = A[i][j];  // i==j means is the diagonal. if we
                              // set B right now ,the  misses and
                              // evictions will increase . because
                              // the cache set of B is same to A.
              index = i;
            }
          }
          if (col_Block == row_Block) {  // just set B on the diagonal. other
            // than shouldn't set the B
            B[index][index] = tmp;
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
void trans(int M, int N, int A[N][M], int B[M][N]) {
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
void registerFunctions() {
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
int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
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
