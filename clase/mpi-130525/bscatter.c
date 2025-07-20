#include <stdio.h>
#include <mpi.h>

void main(int argc, char **argv) {
  int distro_array[4] = {30, 72, 129, 42};
  int scattered_data;

  MPI_Init(&argc, &argv);

  int rank, num_procs;
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Scatter(&distro_array, 1, MPI_INT, &scattered_data, 1, MPI_INT, 0, MPI_COMM_WORLD);
  printf("process has received: %d \n", scattered_data);
  
  MPI_Finalize();
}