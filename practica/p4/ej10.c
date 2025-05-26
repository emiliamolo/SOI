#include <stdio.h>
#include <mpi.h>

#define N 100000007

int arr[N];

int main(int argc, char *argv[]) {
    for (int i = 0; i < N; i++)
        arr[i] = i+1;

    int root_id = 0;
    MPI_Init(&argc, &argv);

    int id, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int block = N / nprocs;
    int l = id*block, r = (id+1)*block;
    if (id == nprocs-1) r = N; // El resto se los queda el último, total no habrá muchos threads...

    long sum = 0;
    for (int i = l; i < r; i++)
        sum += arr[i];

    long sum_reduction;
    MPI_Reduce(&sum, &sum_reduction, 1, MPI_LONG, MPI_SUM, root_id, MPI_COMM_WORLD);

    if (id == root_id) {
        printf("La suma es %ld\n", sum_reduction);
    }

    MPI_Finalize();
    return 0;
}