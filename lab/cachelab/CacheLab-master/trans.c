/*
Author: Yanjie Ze
ID: 519021910706
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
    if(M==32&&N==32)
    {   // using 3 + 8 = 11 variable
    int t0,t1,t2,t3,t4,t5,t6,t7;
    for(int i=0; i<N; i+=8)
        for(int j=0; j<M; j+=8)
            for(int k=i; k<i+8; k++)
            {
                // read all only once
                t0 = A[k][j];
                t1 = A[k][j+1];
                t2 = A[k][j+2];
                t3 = A[k][j+3];
                t4 = A[k][j+4];
                t5 = A[k][j+5];
                t6 = A[k][j+6];
                t7 = A[k][j+7];

                // update
                B[j][k] = t0;
                B[j+1][k] = t1;
                B[j+2][k] = t2;
                B[j+3][k] = t3;
                B[j+4][k] = t4;
                B[j+5][k] = t5;
                B[j+6][k] = t6;
                B[j+7][k] = t7;         
            }
    }
    
    if(M==64&&N==64)
{   // using 4 + 8 = 12 variable
    int t,t1,t2,t3,t4,t5,t6,t7,t8;
    for(int i=0; i<N; i+=8)
        for(int j=0; j<M; j+=8)// blocking 
         {      
             // save and load
                for (t = i; t < i + 4; ++t)
				{
                    //read all only once
					t1 = A[t][j]; t2 = A[t][j+1]; t3 = A[t][j+2]; t4 = A[t][j+3];
					t5 = A[t][j+4]; t6 = A[t][j+5]; t7 = A[t][j+6]; t8 = A[t][j+7];
					
					B[j][t] = t1; B[j+1][t] = t2; B[j+2][t] = t3; B[j+3][t] = t4;
					B[j][t+4] = t5; B[j+1][t+4] = t6; B[j+2][t+4] = t7; B[j+3][t+4] = t8;
				}
				for (t = j; t < j + 4; ++t)
				{
					t1 = A[i+4][t]; t2 = A[i+5][t]; t3 = A[i+6][t]; t4 = A[i+7][t];
					t5 = B[t][i+4]; t6 = B[t][i+5]; t7 = B[t][i+6]; t8 = B[t][i+7];
					
					B[t][i+4] = t1; B[t][i+5] = t2; B[t][i+6] = t3; B[t][i+7] = t4;
					B[t+4][i] = t5; B[t+4][i+1] = t6; B[t+4][i+2] = t7; B[t+4][i+3] = t8;
				}
				for (t = i + 4; t < i + 8; ++t)
				{
					t1 = A[t][j+4]; t2 = A[t][j+5]; t3 = A[t][j+6]; t4 = A[t][j+7];
					B[j+4][t] = t1; B[j+5][t] = t2; B[j+6][t] = t3; B[j+7][t] = t4;
				}        
         }
}

if((N==61&&M==67)||(M==61&&N==67))
{
    // 3+8=11 variable
    int t0,t1,t2,t3,t4,t5,t6,t7;//8 variable
  
    int i,j,k;//3
    for(i=0; i<N-N%16; i+=16)
        for(j=0; j<M-M%16; j+=16)
            for( k=i; k<i+16; k++)
            {
                // read all only once
                t0 = A[k][j];
                t1 = A[k][j+1];
                t2 = A[k][j+2];
                t3 = A[k][j+3];
                t4 = A[k][j+4];
                t5 = A[k][j+5];
                t6 = A[k][j+6];
                t7 = A[k][j+7];

                // update
                B[j][k] = t0;
                B[j+1][k] = t1;
                B[j+2][k] = t2;
                B[j+3][k] = t3;
                B[j+4][k] = t4;
                B[j+5][k] = t5;
                B[j+6][k] = t6;
                B[j+7][k] = t7;  

                // read all only once
                t0 = A[k][j+8];
                t1 = A[k][j+9];
                t2 = A[k][j+10];
                t3 = A[k][j+11];
                t4 = A[k][j+12];
                t5 = A[k][j+13];
                t6 = A[k][j+14];
                t7 = A[k][j+15];

                // update
                B[j+8][k] = t0;
                B[j+9][k] = t1;
                B[j+10][k] = t2;
                B[j+11][k] = t3;
                B[j+12][k] = t4;
                B[j+13][k] = t5;
                B[j+14][k] = t6;
                B[j+15][k] = t7;

            }


    
     // compenstate 
    for (i = 0; i < N; ++i)
		for (j = M-M%16; j < M; ++j)
			{
				t0 = A[i][j];
				B[j][i] = t0;
			}
	for (i = N-N%16; i < N; ++i)
		for (j = 0; j < M; ++j)
			{
				t0 = A[i][j];
				B[j][i] = t0;
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
char trans_simple_desc[] = "Baseline: Simple row-wise scan transpose";
void trans_simple(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}


char blocking4[] = "Blocking 4 transpose";
void transpose_blocking4(int M, int N, int A[N][M], int B[M][N])
{
    for(int i=0; i<N; i+=4)
        for(int j=0; j<M; j+=4)
            for(int k=i; k<i+4; k++)
                for(int m=j; m<j+4; m++)
                    B[m][k] = A[k][m];
}


char blocking8[] = "Blocking 8 transpose";
void transpose_blocking8(int M, int N, int A[N][M], int B[M][N])
{
    for(int i=0; i<N; i+=8)
        for(int j=0; j<M; j+=8)
            for(int k=i; k<i+8; k++)
                for(int m=j; m<j+8; m++)
                    B[m][k] = A[k][m];
}

char blocking16[] = "Blocking 16 transpose";
void transpose_blocking16(int M, int N, int A[N][M], int B[M][N])
{
    for(int i=0; i<N; i+=16)
        for(int j=0; j<M; j+=16)
            for(int k=i; k<i+16; k++)
                for(int m=j; m<j+16; m++)
                    B[m][k] = A[k][m];
}


char blocking4_readall[] = "Blocking 4 Read All Only Once transpose";
void transpose_blocking4_readall(int M, int N, int A[N][M], int B[M][N])
{   // using 3 + 4 = 7 variable
    int t0,t1,t2,t3;
    for(int i=0; i<N; i+=4)
        for(int j=0; j<M; j+=4)
            for(int k=i; k<i+4; k++)
            {
                // read all only once
                t0 = A[k][j];
                t1 = A[k][j+1];
                t2 = A[k][j+2];
                t3 = A[k][j+3];
                

                // update
                B[j][k] = t0;
                B[j+1][k] = t1;
                B[j+2][k] = t2;
                B[j+3][k] = t3;
                         
            }
}


char blocking8_readall[] = "Blocking 8 Read All Only Once transpose";
void transpose_blocking8_readall(int M, int N, int A[N][M], int B[M][N])
{   // using 3 + 8 = 11 variable
    int t0,t1,t2,t3,t4,t5,t6,t7;
    for(int i=0; i<N; i+=8)
        for(int j=0; j<M; j+=8)
            for(int k=i; k<i+8; k++)
            {
                // read all only once
                t0 = A[k][j];
                t1 = A[k][j+1];
                t2 = A[k][j+2];
                t3 = A[k][j+3];
                t4 = A[k][j+4];
                t5 = A[k][j+5];
                t6 = A[k][j+6];
                t7 = A[k][j+7];

                // update
                B[j][k] = t0;
                B[j+1][k] = t1;
                B[j+2][k] = t2;
                B[j+3][k] = t3;
                B[j+4][k] = t4;
                B[j+5][k] = t5;
                B[j+6][k] = t6;
                B[j+7][k] = t7;         
            }
}

char blocking4_readall_improve[] = "Blocking 4 Read All Only Once Improved transpose";
void transpose_blocking4_readall_improve(int M, int N, int A[N][M], int B[M][N])
{   // using 3 + 8 = 11 variable
    int t0,t1,t2,t3, t4,t5,t6,t7;
    for(int i=0; i<N; i+=4)
        for(int j=0; j<M; j+=4)
            for(int k=i; k<i+4; k+=2)
            {
                // read all only once
                t0 = A[k][j];
                t1 = A[k][j+1];
                t2 = A[k][j+2];
                t3 = A[k][j+3];
                
                // read again
                t4 = A[k+1][j];
                t5 = A[k+1][j+1];
                t6 = A[k+1][j+2];
                t7 = A[k+1][j+3];

                 // update
                B[j][k] = t0;
                B[j+1][k] = t1;
                B[j+2][k] = t2;
                B[j+3][k] = t3;

                

                //update      
                B[j][k+1] = t4;
                B[j+1][k+1] = t5;
                B[j+2][k+1] = t6;
                B[j+3][k+1] = t7;
            }
}

char blocking8_readall_improve[] = "Blocking 8 Read All Only Once Improved transpose";
void transpose_blocking8_readall_improve(int M, int N, int A[N][M], int B[M][N])
{   // using 4 + 8 = 12 variable
    int t,t1,t2,t3,t4,t5,t6,t7,t8;
    for(int i=0; i<N; i+=8)
        for(int j=0; j<M; j+=8)
         {
                for (t = i; t < i + 4; ++t)
				{
					t1 = A[t][j]; t2 = A[t][j+1]; t3 = A[t][j+2]; t4 = A[t][j+3];
					t5 = A[t][j+4]; t6 = A[t][j+5]; t7 = A[t][j+6]; t8 = A[t][j+7];
					
					B[j][t] = t1; B[j+1][t] = t2; B[j+2][t] = t3; B[j+3][t] = t4;
					B[j][t+4] = t5; B[j+1][t+4] = t6; B[j+2][t+4] = t7; B[j+3][t+4] = t8;
				}
				for (t = j; t < j + 4; ++t)
				{
					t1 = A[i+4][t]; t2 = A[i+5][t]; t3 = A[i+6][t]; t4 = A[i+7][t];
					t5 = B[t][i+4]; t6 = B[t][i+5]; t7 = B[t][i+6]; t8 = B[t][i+7];
					
					B[t][i+4] = t1; B[t][i+5] = t2; B[t][i+6] = t3; B[t][i+7] = t4;
					B[t+4][i] = t5; B[t+4][i+1] = t6; B[t+4][i+2] = t7; B[t+4][i+3] = t8;
				}
				for (t = i + 4; t < i + 8; ++t)
				{
					t1 = A[t][j+4]; t2 = A[t][j+5]; t3 = A[t][j+6]; t4 = A[t][j+7];
					B[j+4][t] = t1; B[j+5][t] = t2; B[j+6][t] = t3; B[j+7][t] = t4;
				}        
         }
}


char blocking8_readall_improve_shape[] = "Blocking 8 Read All Only Once Improved Random Shape transpose";
void transpose_blocking8_readall_improve_shape(int M, int N, int A[N][M], int B[M][N])
{   
    int t,t1,t2,t3,t4,t5,t6,t7,t8;
    int i, j;
    for(i=0; i<(N-N%8); i+=8)
        for(j=0; j<(M-M%8); j+=8)
         {
                for (t = i; t < i + 4; ++t)
				{
					t1 = A[t][j]; t2 = A[t][j+1]; t3 = A[t][j+2]; t4 = A[t][j+3];
					t5 = A[t][j+4]; t6 = A[t][j+5]; t7 = A[t][j+6]; t8 = A[t][j+7];
					
					B[j][t] = t1; B[j+1][t] = t2; B[j+2][t] = t3; B[j+3][t] = t4;
					B[j][t+4] = t5; B[j+1][t+4] = t6; B[j+2][t+4] = t7; B[j+3][t+4] = t8;
				}
				for (t = j; t < j + 4; ++t)
				{
					t1 = A[i+4][t]; t2 = A[i+5][t]; t3 = A[i+6][t]; t4 = A[i+7][t];
					t5 = B[t][i+4]; t6 = B[t][i+5]; t7 = B[t][i+6]; t8 = B[t][i+7];
					
					B[t][i+4] = t1; B[t][i+5] = t2; B[t][i+6] = t3; B[t][i+7] = t4;
					B[t+4][i] = t5; B[t+4][i+1] = t6; B[t+4][i+2] = t7; B[t+4][i+3] = t8;
				}
				for (t = i + 4; t < i + 8; ++t)
				{
					t1 = A[t][j+4]; t2 = A[t][j+5]; t3 = A[t][j+6]; t4 = A[t][j+7];
					B[j+4][t] = t1; B[j+5][t] = t2; B[j+6][t] = t3; B[j+7][t] = t4;
				}        
         }

    // compenstate 
    for (i = 0; i < N; ++i)
		for (j = (M-M%8); j < M; ++j)
			{
				t = A[i][j];
				B[j][i] = t;
			}
	for (i = (N-N%8); i < N; ++i)
		for (j = 0; j < M; ++j)
			{
				t = A[i][j];
				B[j][i] = t;
			}

}

char blocking16_readall_shape[] = "Blocking 16 Read All Random Shape transpose";
void transpose_blocking16_readall_shape(int M, int N, int A[N][M], int B[M][N])
{
    int t0,t1,t2,t3,t4,t5,t6,t7;
  
    int i,j;
    for(i=0; i<N-N%16; i+=16)
        for(j=0; j<M-M%16; j+=16)
            for(int k=i; k<i+16; k++)
            {
                // read all only once
                t0 = A[k][j];
                t1 = A[k][j+1];
                t2 = A[k][j+2];
                t3 = A[k][j+3];
                t4 = A[k][j+4];
                t5 = A[k][j+5];
                t6 = A[k][j+6];
                t7 = A[k][j+7];

                // update
                B[j][k] = t0;
                B[j+1][k] = t1;
                B[j+2][k] = t2;
                B[j+3][k] = t3;
                B[j+4][k] = t4;
                B[j+5][k] = t5;
                B[j+6][k] = t6;
                B[j+7][k] = t7;  

                // read all only once
                t0 = A[k][j+8];
                t1 = A[k][j+9];
                t2 = A[k][j+10];
                t3 = A[k][j+11];
                t4 = A[k][j+12];
                t5 = A[k][j+13];
                t6 = A[k][j+14];
                t7 = A[k][j+15];

                // update
                B[j+8][k] = t0;
                B[j+9][k] = t1;
                B[j+10][k] = t2;
                B[j+11][k] = t3;
                B[j+12][k] = t4;
                B[j+13][k] = t5;
                B[j+14][k] = t6;
                B[j+15][k] = t7;

            }


    
     // compenstate 
    for (i = 0; i < N; ++i)
		for (j = M-M%16; j < M; ++j)
			{
				t0 = A[i][j];
				B[j][i] = t0;
			}
	for (i = N-N%16; i < N; ++i)
		for (j = 0; j < M; ++j)
			{
				t0 = A[i][j];
				B[j][i] = t0;
			}
}

char blocking17_shape[] = "Blocking 17 Random Shape transpose";
void transpose_blocking17_shape(int M, int N, int A[N][M], int B[M][N])
{
    int t;
    for(int i=0; i<N-N%17; i+=17)
        for(int j=0; j<(M-M%17); j+=17)
            for(int k=i; k<i+17; k++)
                for(int w=j; w<j+17; w++)
                   { 
                       t = A[k][w];
                        B[w][k]=t;
                   }

    
     // compenstate 
    for (int i = 0; i < N; ++i)
		for (int j = M-M%17; j < M; ++j)
			{
				t = A[i][j];
				B[j][i] = t;
			}
	for (int i = N-N%17; i < N; ++i)
		for (int j = 0; j < M; ++j)
			{
				t = A[i][j];
				B[j][i] = t;
			}
}

char blocking15_shape[] = "Blocking 15 Random Shape transpose";
void transpose_blocking15_shape(int M, int N, int A[N][M], int B[M][N])
{
    int t;
    int m=M-M%15;
    int n=N-N%15;
    for(int i=0; i<n; i+=15)
        for(int j=0; j<m; j+=15)
            for(int k=i; k<i+15; k++)
                for(int w=j; w<j+15; w++)
                   { 
                       t = A[k][w];
                        B[w][k]=t;
                   }

    
     // compenstate 
    for (int i = 0; i < N; ++i)
		for (int j = m; j < M; ++j)
			{
				t = A[i][j];
				B[j][i] = t;
			}
	for (int i = n; i < N; ++i)
		for (int j = 0; j < M; ++j)
			{
				t = A[i][j];
				B[j][i] = t;
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
    // registerTransFunction(trans_simple, trans_simple_desc); 
    // registerTransFunction(transpose_blocking4, blocking4);
    // registerTransFunction(transpose_blocking8, blocking8);
    // registerTransFunction(transpose_blocking16, blocking16);
    // registerTransFunction(transpose_blocking4_readall, blocking4_readall);
    // registerTransFunction(transpose_blocking8_readall, blocking8_readall);
    // registerTransFunction(transpose_blocking4_readall_improve, blocking4_readall_improve);
    // registerTransFunction(transpose_blocking8_readall_improve, blocking8_readall_improve);
    // registerTransFunction(transpose_blocking8_readall_improve_shape, blocking8_readall_improve_shape);
    // registerTransFunction(transpose_blocking17_shape, blocking17_shape);
    // registerTransFunction(transpose_blocking15_shape, blocking15_shape);
    // registerTransFunction(transpose_blocking16_readall_shape, blocking16_readall_shape);

    

  

   

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

