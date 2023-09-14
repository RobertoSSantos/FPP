#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    const int N = 2;  // Matrix size is N x N
    int matrix[N][N];
    int local_data;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != N*N) {
        if (rank == 0) {
            printf("This example requires %d processes.\n", N*N);
        }
        MPI_Finalize();
        return 0;
    }

    // Each process initializes its portion of the matrix with its rank
    local_data = rank;

    int row = rank / N;
    int col = rank % N;
    matrix[row][col] = local_data;

    // Barrier synchronization
    MPI_Barrier(MPI_COMM_WORLD);

    // Root process gathers the matrix data from all processes
    int received_data[N*N];
    MPI_Gather(&local_data, 1, MPI_INT, received_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Root process prints the matrix
    if (rank == 0) {
        printf("Matrix:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", received_data[i*N + j]);
            }
            printf("\n");
        }
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
