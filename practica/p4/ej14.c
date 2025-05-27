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
    
    if (!id){
        int A[N][N], x[N];
        int v[N] = {1,1,1};
        for(int i=0;i<N;i++){
            for(int j=0; j<N; j++){
                A[i][j] = 0;
            }
            A[i][i] = 1;
        }
        
        MPI_Scatter(A, block, MPI_INT, A, 1, MPI_INT, 0,MPI_COMM_WORLD);
        MPI_Scatter(A, block, MPI_INT, A, 1, MPI_INT, 0,MPI_COMM_WORLD);
        MPI_Bcast(v, N, MPI_INT, 0, MPI_COMM_WORLD);

    } else{
        int block = N / nprocs, rest = N % nprocs;
        int localA[block+(id<rest)][N], v[N];
    }
    
    //int l = min(id,rest)*(block+1) + max(0,id-rest)*block, r = l + block + (id < rest);
    
    /*Leer A y v*/
    for (i=l;i<r;i++) {
        x[i]=0;
        for (j=0;j<N;j++)
            x[i] += A[i][j]*v[j];
    }

    // TODO: TERMINAR!
    
    if(!id) {
        int xf[N];
        MPI_Gather(x+l, r-l, MPI_INT, xf, N, MPI_INT, 0, MPI_COMM_WORLD);
        for(int i=0; i <N; i++)
            printf("%d\n", xf[i]);
    }
    else {

        if (r-l > 0)
            MPI_Gather(x+l, r-l, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}