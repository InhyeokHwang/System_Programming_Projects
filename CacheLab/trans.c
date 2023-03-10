/*inhyeok hwang 2021058995*/

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
	int row_block, col_block;
	int row, col;
	int n0, n1, n2, n3, n4, n5, n6, n7;

	if(M == 32 && N == 32){
		for(col_block = 0; col_block < M; col_block += 8){
			for(row_block = 0; row_block < N; row_block += 8){
				for(row = row_block; row < row_block + 8; row++){
					for(col = col_block; col < col_block + 8; col++){
						if(row != col) {
							B[col][row] = A[row][col];
						}else{
							n0 = A[row][col];
							n1 = row;
						}
					}
					if(row_block == col_block){
						B[n1][n1] = n0;
					}
				}
			}
		}
	}else if(M == 64 && N == 64){
		for(col_block = 0; col_block < M; col_block += 8){
			for(row_block = 0; row_block < N; row_block += 8){
				for(row = row_block; row < row_block + 4; row++){
					n0 = A[row][col_block];
					n1 = A[row][col_block + 1];
					n2 = A[row][col_block + 2];
					n3 = A[row][col_block + 3];
					n4 = A[row][col_block + 4];
					n5 = A[row][col_block + 5];
					n6 = A[row][col_block + 6];
					n7 = A[row][col_block + 7];
					B[col_block][row] = n0;
					B[col_block + 1][row] = n1;
					B[col_block + 2][row] = n2;
					B[col_block + 3][row] = n3;
					B[col_block][row + 4] = n4;
					B[col_block + 1][row + 4] = n5;
					B[col_block + 2][row + 4] = n6;
					B[col_block + 3][row + 4] = n7;
				}
				for(col = col_block; col < col_block + 4; col++){
					n4 = A[row_block + 4][col];
					n5 = A[row_block + 5][col];
					n6 = A[row_block + 6][col];
					n7 = A[row_block + 7][col];

					n0 = B[col][row_block + 4];
					n1 = B[col][row_block + 5];
					n2 = B[col][row_block + 6];
					n3 = B[col][row_block + 7];

					B[col][row_block + 4] = n4;
					B[col][row_block + 5] = n5;
					B[col][row_block + 6] = n6;
					B[col][row_block + 7] = n7;

					B[col + 4][row_block] = n0;
					B[col + 4][row_block + 1] = n1;
					B[col + 4][row_block + 2] = n2;
					B[col + 4][row_block + 3] = n3;

					B[col + 4][row_block + 4] = A[row_block + 4][col + 4];
					B[col + 4][row_block + 5] = A[row_block + 5][col + 4];
					B[col + 4][row_block + 6] = A[row_block + 6][col + 4];
					B[col + 4][row_block + 7] = A[row_block + 7][col + 4];
				}
			}
		}
	}else if(M == 61 && N == 67){
		for(col_block = 0; col_block < M; col_block += 16){
			for(row_block = 0; row_block < N; row_block += 16){
				for(row = row_block; (row < N) && (row < row_block + 16); row++){
					for(col = col_block; (col < M) && (col < col_block + 16); col++){
						if(row != col){
							B[col][row] = A[row][col];
						}else{
							n0 = A[row][col];
							n1 = row;
						}
					}
					if(row_block == col_block){
						B[n1][n1] = n0;
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

