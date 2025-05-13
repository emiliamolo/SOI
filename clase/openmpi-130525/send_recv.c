#include <stdio.h>
#include <mpi.h>
#include <string.h>

void main(int argc, char **argv) {
  int num_procs, my_id;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  char buf[10];
  if (!my_id) {
    printf("Soy 0, mando HOLA a 1.\n");
    strcpy(buf, "HOLA");
    MPI_Send(buf, 5, MPI_CHAR, my_id+1, my_id, MPI_COMM_WORLD);
  }
  else if (my_id == num_procs-1) {
    MPI_Recv(buf, 5, MPI_CHAR, my_id-1, my_id-1, MPI_COMM_WORLD, NULL);
    printf("Recibí \"%s\" de %d. Soy el último.\n",buf,my_id-1);
  }
  else {
    MPI_Recv(buf, 5, MPI_CHAR, my_id-1, my_id-1, MPI_COMM_WORLD, NULL);
    printf("Recibí \"%s\" de %d. Se lo mando a %d.\n",buf,my_id-1,my_id+1);
    MPI_Send(buf, 5, MPI_CHAR, my_id+1, my_id, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}