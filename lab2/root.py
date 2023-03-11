#!/usr/bin/env python
from mpi4py import MPI
import random
import numpy as np
import sys


def is_in_circle(x, y):
    return x ** 2 + y ** 2 < 1


def random_points(n):
    X = np.random.rand(n, 2)
    in_circle = np.count_nonzero(is_in_circle(X[:, 0], X[:, 1]))
    return np.array(in_circle, dtype=np.longlong)


if __name__ == '__main__':
    comm = MPI.COMM_WORLD
    size = comm.size()
    world_rank = comm.Get_rank()
    points_per_proc = 1e6

    in_circle = random_points(points_per_proc)
    result = np.array(0, dtype=np.longlong)

    comm.Reduce(in_circle, result, op=MPI.SUM, root=0)

    if world_rank == 0:
        print(f'Pi: {4 * in_circle / (points_per_proc * size)}')

    comm.Disconnect()
