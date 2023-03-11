#!/usr/bin/env python
from mpi4py import MPI
import random 
import numpy as np

def is_in_circle(x,y):
    return x ** 2 + y ** 2 < 1

def random_points(n):
    X = np.random.rand(n, 2)
    in_circle = np.count_nonzero(is_in_circle(X[:, 0], X[:, 1]))
    return in_circle, n



if __name__ = '__main__':
    comm = MPI.Comm.Get_parent()
    size = comm.Get_size()
    rank = comm.Get_rank()
    n = 1000000
    circle, n = random_points(n)
    N = numpy.array(0, dtype='i')
    comm.Bcast([N, circle], root=0)
