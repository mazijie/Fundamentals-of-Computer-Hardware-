/* 
 * trans.c - Matrik1 transpose B = A^T
 *
 * Your transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * The transpose function is evaluated by counting the number of misses
 * on a 512B direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */

void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    /*
     * Please define your local variables here. 
     * You are allowed to define at most 12 local variables.
    */
    int i,j,k;
    int v0, v1, v2, v3, v4, v5, v6, v7;
    
    if(M == 16){ //16k116 matrix
        for(i=0;i<M;i+=8)
        {
            for(j=0;j<M;j+=8)
            {
                for(k=0;k<8;k++)
                {
                    v0=A[i+k][j];
                    v1=A[i+k][j+1];
                    v2=A[i+k][j+2];
                    v3=A[i+k][j+3];
                    v4=A[i+k][j+4];
                    v5=A[i+k][j+5];
                    v6=A[i+k][j+6];
                    v7=A[i+k][j+7];
                    B[j][i+k]=v0;
                    B[j+1][i+k]=v1;
                    B[j+2][i+k]=v2;
                    B[j+3][i+k]=v3;
                    B[j+4][i+k]=v4;
                    B[j+5][i+k]=v5;
                    B[j+6][i+k]=v6;
                    B[j+7][i+k]=v7;
                }
            }
        }
    }
    else if(M == 32){ //32x32 matrix
    for(i = 0; i < N; i += 8) {
        for(j = 0; j < M; j += 8) {
            for(k = i; k < i + 4; k++) {
                v0 = A[k][j];
                v1 = A[k][j + 1];
                v2 = A[k][j + 2];
                v3 = A[k][j + 3];
                v4 = A[k][j + 4];
                v5 = A[k][j + 5];
                v6 = A[k][j + 6];
                v7 = A[k][j + 7];
                B[j][k] = v0;
                B[j + 1][k] = v1;
                B[j + 2][k] = v2;
                B[j + 3][k] = v3;
                B[j][k + 4] = v4;
                B[j + 1][k + 4] = v5;
                B[j + 2][k + 4] = v6;
                B[j + 3][k + 4] = v7;
            }

            for(k = j; k < j + 4; k++){
                v0 = B[k][i + 4];
                v1 = B[k][i + 5];
                v2 = B[k][i + 6];
                v3 = B[k][i + 7];
                v4 = A[i + 4][k];
                v5 = A[i + 5][k];
                v6 = A[i + 6][k];
                v7 = A[i + 7][k];
                B[k][i + 4] = v4;
                B[k][i + 5] = v5;
                B[k][i + 6] = v6;
                B[k][i + 7] = v7;
                B[k + 4][i] = v0;
                B[k + 4][i + 1] = v1;
                B[k + 4][i + 2] = v2;
                B[k + 4][i + 3] = v3;
            }
			for(k = i + 4; k < i + 8; k++){
                v0 = A[k][j + 4];
                v1 = A[k][j + 5];
                v2 = A[k][j + 6];
                v3 = A[k][j + 7];
                B[j + 4][k] = v0;
                B[j + 5][k] = v1;
                B[j + 6][k] = v2;
                B[j + 7][k] = v3;
            }
        }
    } 
}
}




