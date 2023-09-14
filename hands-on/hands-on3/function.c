#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    double coef[4], total, x;
    double coef_rec, partial;
    int operations_rec, operations_order[] = {1,2};
    char c;

    int numberOfProcessors, id, to, from, tag = 1000;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, & numberOfProcessors);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Status status;

    switch(id){
        case 0:
            // Master
            printf ("\nf(x) = a*x^3 + b*x^2 + c*x + d\n");

            coef[0] = 1;
            coef[1] = 2;  
            coef[2] = 3;
            coef[3] = 4;
  
            printf("\nf(x)=%lf*x^3+%lf*x^2+%lf*x+%lf\n", coef[0], coef[1], coef[2], coef[3]);

            x = 10;

            for(to = 1; to < numberOfProcessors; to++){
                MPI_Send(&coef[to-1], 1, MPI_DOUBLE, to, tag, MPI_COMM_WORLD);
                MPI_Send(&x, 1, MPI_DOUBLE, to, tag, MPI_COMM_WORLD);
                MPI_Send(&operations_order[to-1], 1, MPI_INT, to, tag, MPI_COMM_WORLD);
            }

            total = (coef[2]* x + coef[3]);

            for(to = 1; to < numberOfProcessors; to++){
                MPI_Recv(&partial, 1, MPI_DOUBLE, to, tag, MPI_COMM_WORLD, &status);
                total += partial;
            }

            printf("\nf(%lf) = %lf*x^3 + %lf*x^2 + %lf*x + %lf = %lf\n", x, coef[0], coef[1], coef[2], coef[3], total);
            
            break;

        default:
            //Workers

            MPI_Recv(&coef_rec, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
            MPI_Recv(&x, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
            MPI_Recv(&operations_rec, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

            switch(operations_rec){
                case 1:
                    partial = (coef_rec * x * x * x);
                    break;
                case 2:
                    partial = (coef_rec * x * x);
                    break;
            }

            MPI_Send(&partial, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
        
    }
    
    MPI_Finalize();
    return 0;
}
