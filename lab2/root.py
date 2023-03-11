#!/usr/bin/env python
from mpi4py import MPI
import random 
import numpy as np

comm = MPI.COMM_SELF.Spawn(sys.executable,
                           args=['monte_carlo.py'],
                           maxprocs=5)

N = numpy.array(100_000_000, dtype=np.byte)
comm.Bcast([N, MPI.MPI_LONG], root=MPI.ROOT)
PI = numpy.array(0.0, 'd')
comm.Reduce(None, [PI, MPI.DOUBLE],
            op=MPI.SUM, root=MPI.ROOT)

comm.Disconnect()