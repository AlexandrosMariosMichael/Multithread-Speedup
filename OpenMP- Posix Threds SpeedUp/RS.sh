#!/bin/bash
A=1000
B=1000
l=1000
nt=4
gcc SC.c -o SC
gcc -fopenmp  SCopenmpa.c -o SCopenmpa
gcc -fopenmp  SCopenmpb.c -o SCopenmpb
gcc -fopenmp  SCopenmpc.c -o SCopenmpc
gcc -pthread  SCpthreadsa.c -o SCpthreadsa
gcc -pthread  SCpthreadsb.c -o SCpthreadsb
gcc -pthread  SCpthreadsc.c -o SCpthreadsc
echo $A $B $l | ./SC
echo $A $B $l $nt | ./SCopenmpa
echo $A $B $l $nt | ./SCopenmpb
echo $A $B $l $nt | ./SCopenmpc
echo $A $B $l $nt | ./SCpthreadsa
echo $A $B $l $nt | ./SCpthreadsb
echo $A $B $l $nt | ./SCpthreadsc



