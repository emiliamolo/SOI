#include <stdio.h>
#include <mpi.h>
#include <string.h>

void main(int argc, char **argv) {
    int num_procs, my_id;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  my_id++; // indexado en 1

    int s1 = 0, s2 = 0;
    int rem1 = my_id*2, rem2 = my_id*2+1;
    fflush(stdout);
    if (rem1 <= num_procs) 
        MPI_Recv(&s1, 1, MPI_INT, rem1-1, rem1, MPI_COMM_WORLD, NULL); // TODO: Los mensajes esperan?
    if (rem2 <= num_procs)
        MPI_Recv(&s2, 1, MPI_INT, rem2-1, rem2, MPI_COMM_WORLD, NULL);
    
    if (my_id > 1) {
        int sum = s1+s2+(my_id-1); // my_id original
        int p = my_id/2;
        MPI_Send(&sum, 1, MPI_INT, p-1, my_id, MPI_COMM_WORLD);
    }
    else {
        printf("Sumamos %d!\n",s1+s2); // +0
    }

    MPI_Finalize();
}

// Es robusta porque se verifica en cada paso de recibir si los hijos son ids de procesos válidos, y funciona en lg porque toma la estructura de un Heap.