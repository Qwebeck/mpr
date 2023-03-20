#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=10:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgmpr23-cpu 

module add scipy-bundle/2021.10-foss-2021b
python runner.py
