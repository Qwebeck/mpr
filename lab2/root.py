#!/usr/bin/env python
from mpi4py import MPI
import random
import numpy as np
import sys

points_per_proc = 1e6
proc_count = 5
comm = MPI.COMM_SELF.Spawn(sys.executable,
                           args=['monte_carlo.py'],
                           maxprocs=proc_count,
                           root=0)

IN_CIRCLE = np.array(0.0, dtype=np.longlong)
comm.Reduce(None, IN_CIRCLE, op=MPI.SUM, root=MPI.ROOT)
print(f'Pi: {4 * IN_CIRCLE / points_per_proc}')

comm.Disconnect()
