#include <stdio.h>
#include <mpi.h>
#define ORDER 4

void printMatrix (int m[][ORDER]) 
{
  int i, j;
  for(i = 0; i < ORDER; i++) {
    printf ("| ");
    for (j = 0; j < ORDER; j++) {
      printf ("%3d ", m[i][j]);
    }
    printf ("|\n");
  }
  printf ("\n");
}

int main(int argc, char **argv) {
    int k[3] = {100,200,300};
    int matrix[ORDER][ORDER], h, i, j;

    int numberOfProcessors, id, to, from, tag = 1000;
    int matrix_rec[ORDER][ORDER], k_rec,operations_rec, operations[] = {1,2};

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, & numberOfProcessors);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Status status;

    switch(id) {
      case 0: // MASTER
        // CREATE MATRIX
        for(i = 0; i < ORDER; i++) {
          for(j = 0; j < ORDER; j++) {
            if(i == j)
              matrix[i][j] = i + j + 1;
            else if (i == (j + 1)) {
              matrix[i][j] = i + j + 1;
              matrix[j][i] = matrix[i][j];
            } else
              matrix[i][j] = 0;
          }
        }

        printMatrix(matrix);

        for(to = 1; to < numberOfProcessors; to++){
          MPI_Send(&matrix, (ORDER * ORDER), MPI_INT, to, tag, MPI_COMM_WORLD);
          MPI_Send(&k[to-1], 1, MPI_INT, to, tag, MPI_COMM_WORLD);
          MPI_Send(&operations[to], 1, MPI_INT, to, tag, MPI_COMM_WORLD);
        }

        for(i = 0; i < ORDER; i++)
          matrix[i][i + 1] += k[2];

        for(h = 1; i < numberOfProcessors; i++){
          MPI_Recv(&matrix_rec, (ORDER * ORDER), MPI_INT, h, tag, MPI_COMM_WORLD, &status);
          for(i = 0; i < ORDER; i++){
            for(j = 0; j < ORDER; j++){
              matrix[i][j] = matrix[i][j] + matrix_rec[i][j];
            }
          }
        }

        printMatrix(matrix);
        break;
      default:
        MPI_Recv(&matrix_rec, (ORDER * ORDER), MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&k_rec, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&operations_rec, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

        switch(operations_rec){
          case 0:
            for(i = 0; i < ORDER; i++) {
              matrix_rec[i][i] += k_rec;
            }
            break;
          case 1:
            for(i = 0; i < ORDER; i++) {
              matrix_rec[i + 1][i] += k_rec;
            }
        }

        MPI_Send(&matrix_rec, (ORDER*ORDER), MPI_INT, 0, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
