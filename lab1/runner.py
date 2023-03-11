#!/usr/bin/python
from mpi4py import MPI
from itertools import product
import os


allnodes_files = [
    'one_node',
    'two_nodes'
]

send_files = [
    'send',
    'bsend'
]

if __name__ == '__main__':
    for (allnodes, send) in product(allnodes_files, send_files):
        print('starting experiment: {} {}'.format(allnodes, send))
        write_to = '{}_{}.txt'.format(allnodes, send)
        os.system(
            'mpiexec -machinefile ./{0} -np 2 ./{1}.py ./{2}'.format(allnodes, send, write_to))
