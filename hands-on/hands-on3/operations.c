#include <stdio.h> 
#include <mpi.h>
#define SIZE 12

int main (int argc, char **argv) {
    int i, sum = 0, subtraction = 0, mult = 1; 
    int array[SIZE];
    char operations[] = {'+','-','*'};
    char operationsRec;
    int numberOfProcessors, id, to, from, tag = 1000;
    int result, values;

    /******Initializing MPI******/

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcessors);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Status status;

    switch(id){
        case 0:
            for(i=0;i<SIZE;i++){
                array[i] = i + 1;
                printf("array[%d] = %d\n", i, array[i]);
            }
            printf("\n");

            for(to = 1; to < numberOfProcessors; to++){
                MPI_Send(&array, SIZE, MPI_INT, to, tag, MPI_COMM_WORLD);
                MPI_Send(&operations[to-1], 1, MPI_CHAR, to, tag, MPI_COMM_WORLD);
            }

            for(to = 1; to < numberOfProcessors; to++){
                MPI_Recv(&result, 1, MPI_INT, to, tag, MPI_COMM_WORLD, &status);
                MPI_Recv(&operationsRec, 1, MPI_CHAR, to, tag, MPI_COMM_WORLD, &status);
                printf ("(%c) = %d\n", operationsRec, result);
            }

        break;

        default:
            MPI_Recv(&array, SIZE, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
            MPI_Recv(&operationsRec, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);

            switch(operationsRec) {
                case '+':
                    values = 0;
                    for(i = 0; i < SIZE; i++)
                        values += array[i];
                    break;
                case '-':
                    values = 0;
                    for(i = 0; i < SIZE; i++)
                        values -= array[i];
                    break;
                case '*':
                    values = 0;
                    for(i = 0; i < SIZE; i++)
                        values *= array[i];
                    break;
            }

            MPI_Send(&values, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
            MPI_Send(&operationsRec, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);

    }
    
    return 0;

}
