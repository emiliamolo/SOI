#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 3

int min(int a, int b){
    if (a < b) return a;
    return b;
}
int max(int a, int b){
    if (a > b) return a;
    return b;
}


int main(int argc, char **argv)
{
    int i, j, id, nprocs;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    int filas = N/nprocs;
    int localA[filas][N], localX[N];
    if (!id){
        int A[N][N], x[N];
        int v[N] = {1,1,1};
        for(int i=0;i<N;i++){
            for(int j=0; j<N; j++){
                A[i][j] = 0;
            }
            A[i][i] = 1;
        }
        
        MPI_Scatter(A, N, MPI_INT, A, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(v, N, MPI_INT, 0, MPI_COMM_WORLD);

        MPI_Gather(A, N, MPI_INT, x, N, MPI_INT, 0,MPI_COMM_WORLD);
        for(int i=0;i<N;i++){
            printf("x[%d] = %d\n", i, x[i]);
        }
    }
     else {
        MPI_Gather(NULL, 0, MPI_INT, localA, filas, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Gather(NULL, 0, MPI_INT, localV, N, MPI_INT, 0, MPI_COMM_WORLD);
        int x[filas];
        for (i=0;i<filas;i++) {
            x[i]=0;
            for (j=0;j<N;j++)
                x[i] += localA[i][j]*localV[j];
        }
        MPI_Gather(x, filas, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}