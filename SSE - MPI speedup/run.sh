#!/bin/bash

#P=2

gcc  -std=c99 -o Serial Serial.c
gcc -std=c99 -msse4.2 -o SSEoptimise SSEoptimise.c 
mpicc -std=c99  -o SSE+MPIoptimise SSE+MPIoptimise.c 

echo
N=100

./Serial $N
./SSEoptimise $N

mpiexec -n 2 ./SSE+MPIoptimise $N
mpiexec -n 4 ./SSE+MPIoptimise $N
mpiexec -n 8 ./SSE+MPIoptimise $N
echo
N=1000

./Serial $N
./SSEoptimise $N

mpiexec -n 2 ./SSE+MPIoptimise $N
mpiexec -n 4 ./SSE+MPIoptimise $N
mpiexec -n 8 ./SSE+MPIoptimise $N
echo
N=10000

./Serial $N
./SSEoptimise $N

mpiexec -n 2 ./SSE+MPIoptimise $N
mpiexec -n 4 ./SSE+MPIoptimise $N
mpiexec -n 8 ./SSE+MPIoptimise $N
echo
N=100000

./Serial $N
./SSEoptimise $N

mpiexec -n 2 ./SSE+MPIoptimise $N
mpiexec -n 4 ./SSE+MPIoptimise $N
mpiexec -n 8 ./SSE+MPIoptimise $N