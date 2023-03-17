#!/usr/bin/env python
from mpi4py import MPI
import numpy as np
from pathlib import Path
import sys


def is_in_circle(x, y):
    return x ** 2 + y ** 2 < 1


def random_points(n):
    total_in_circle = 0
    max_points_in_memory = int(1e6)
    for _ in range(int(n / max_points_in_memory)):
        X = np.random.rand(max_points_in_memory, 2)
        in_circle = np.count_nonzero(is_in_circle(X[:, 0], X[:, 1]))
        total_in_circle += in_circle
    return np.array(total_in_circle, dtype=np.ulonglong)


if __name__ == '__main__':
    comm = MPI.COMM_WORLD
    size = comm.size
    world_rank = comm.Get_rank()
    points_per_proc = int(float(sys.argv[1]))
    write_to = Path(sys.argv[2])

    elapsed_time = MPI.Wtime()
    in_circle = random_points(points_per_proc)
    result = np.array(0, dtype=np.ulonglong)
    comm.Reduce(in_circle, result, op=MPI.SUM, root=0)
    if world_rank == 0:
        wall_time = MPI.Wtime() - elapsed_time
        with write_to.open('a+') as f:
            f.write(f'{wall_time}\n')
