#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=03:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgmpr23-cpu 

gcc -Wall bf_bucket_working.c -o bf_out -fopenmp -lm
./bf_out

gcc -Wall fk_bucket_working.c -o fk_out -fopenmp -lm
./fk_out