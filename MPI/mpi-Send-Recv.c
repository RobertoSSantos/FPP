#include <stdio.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
  int a[8] = {1,2,3,4,5,6,7,8};
  int b[8];

  int numOfProcessors, rank, dest,i, tag = 1000;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numOfProcessors);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Status status;

  if(rank == 0){

      for (dest = 1; dest < numOfProcessors; dest++)
        MPI_Send(&a, 8, MPI_INT, dest, tag, MPI_COMM_WORLD);

  }else{

     MPI_Recv(&b, 8, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

     for(i = 0; i < 8; i++)
       printf("%d\t", b[i]);

     printf("\n");

  }

  MPI_Finalize();

  return 0;

}
