#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 7

int main(int argc, char **argv)
{
    int i, j, id, nprocs;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    
    int bloque = N/nprocs, resto = N%nprocs;
    int sendcounts[nprocs];
    int desplazamientos[nprocs];
    int offset = 0;
    for(int i=0; i < nprocs; i++){
        sendcounts[i] = (bloque + (i<resto)) * N;
        desplazamientos[i] = offset;
        offset += sendcounts[i];
    }
    int filas = sendcounts[id]/N;


    int localA[filas][N], localX[N];
    int v[N];
    if (!id){
        int A[N][N], x[N];
        for(int i=0;i<N;i++){
            v[i] = 1;
            for(int j=0; j<N; j++){
                A[i][j] = 0;
            }
            A[i][i] = 1;
        }
        int recvcounts[nprocs];
        int displs_x[nprocs]; 
        offset = 0;
        for (int i = 0; i < nprocs; i++) {
            recvcounts[i] = sendcounts[i] / N; // cada fila produce un solo entero en x
            displs_x[i] = offset;
            offset += recvcounts[i];
        }

        MPI_Scatterv(A, sendcounts, desplazamientos, MPI_INT, localA, sendcounts[id], MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(v, N, MPI_INT, 0, MPI_COMM_WORLD); // envio v a todos
        
        // el proceso root tambien calcula una parte
        for (i=0;i<filas;i++) {
            localX[i]=0;
            for (j=0;j<N;j++)
                localX[i] += localA[i][j]*v[j];
        }


        MPI_Gatherv(localX, filas, MPI_INT, x, recvcounts, displs_x, MPI_INT, 0, MPI_COMM_WORLD);
        for(int i=0;i<N;i++){ 
            printf("x[%d] = %d\n", i, x[i]);
        }
    }
     else {
        //  scatter para recibir lo que envie con scatter en root
        MPI_Scatterv(NULL, sendcounts, desplazamientos, MPI_INT, localA, sendcounts[id], MPI_INT, 0, MPI_COMM_WORLD);
        // bcast para recibir lo que envie con bcast en root
        MPI_Bcast(v, N, MPI_INT, 0, MPI_COMM_WORLD);
        for (i=0;i<filas;i++) {
            localX[i]=0;
            for (j=0;j<N;j++)
                localX[i] += localA[i][j]*v[j];
        }
        // hago gather para enviar al proceso 0/root
        MPI_Gatherv(localX, filas, MPI_INT, NULL, NULL, NULL, MPI_INT, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}