#!/bin/sh

for i in 100 200 300 400 500 600 700 800 900 1000
do 
    ./mm-openMP "$i"
    OMP_NUM_THREADS=8  ./mm-openMP  "$i"
    mpirun -np 6 ./mm-mpi "$i"
    mpirun -np 6 ./mm-mpi+openmp  "$" 8 
done
