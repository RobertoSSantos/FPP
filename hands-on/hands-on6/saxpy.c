#include <stdio.h>
#include <stdlib.h>

void saxpy(int n,  float *x, float *y)
{
 for (int i=0; i < n ; ++i)
  y[i] = x[i] + y[i];
}

void printVector(float *vector, int n)
{

 for (int i=0; i < n ; ++i)
  printf("%1.0f\t", vector[i]);

  printf("\n\n");
}

void generateVector(float *vector, int n)
{
 for (int i=0; i < n ; ++i)
  vector[i] = i + 1;
}

int main(int argc, char *argv[])
{
  int n = atoi(argv[1]);
  float *x,*y;

  x = (float*) malloc(sizeof(float) * n);
  y = (float*) malloc(sizeof(float) * n);

  generateVector(x, n);
  printVector(x, n);

  generateVector(y, n);
  printVector(y, n);

  saxpy(n, x, y);
  printVector(y, n);

  free(x);
  free(y);

  return 0;

}
