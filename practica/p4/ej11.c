#include <stdio.h>
#include <mpi.h>
#include <string.h>

void main(int argc, char **argv) {
  int num_procs, my_id;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  if (!my_id) {
    printf("N procesos: %d \n", num_procs);
    MPI_Send(&my_id, 1, MPI_INT, my_id+1, my_id, MPI_COMM_WORLD);
  }
  else if (my_id == num_procs-1) {
    int suma;
    MPI_Recv(&suma, 1, MPI_INT, my_id-1, my_id-1, MPI_COMM_WORLD, NULL);
    suma += my_id;
    printf("La suma es: %d \n", suma);
  }
  else {
    int suma;
    MPI_Recv(&suma, 1, MPI_INT, my_id-1, my_id-1, MPI_COMM_WORLD, NULL);
    suma += my_id;
    MPI_Send(&suma, 1, MPI_INT, my_id+1, my_id, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}