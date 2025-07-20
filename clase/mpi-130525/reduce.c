#include <stdio.h>
#include <mpi.h>

void main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, root_rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int reduction_result = 0;
  MPI_Reduce(&rank, &reduction_result, 1, MPI_INT, MPI_SUM, root_rank, MPI_COMM_WORLD);

  if (rank == root_rank) {
    printf("The sum of all ranks is %d.\n", reduction_result);
  }
  
  MPI_Finalize();
}