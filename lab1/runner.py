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
        print(f'starting experiment: {allnodes} {send_files}')
        write_to = f'{allnodes}_{send}.txt'
        os.system(
            f'mpiexec -machinefile ./{allnodes} -np 2 ./{send}.py ./{write_to}')
