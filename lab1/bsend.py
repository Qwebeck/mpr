#!/usr/bin/python
from mpi4py import MPI
import numpy as np
import sys


def send_data(char_count, trials):
    data = np.ones(char_count + MPI.BSEND_OVERHEAD, dtype=np.byte)
    sent = 0
    world_rank = comm.Get_rank()
    partner_rank = (world_rank + 1) % 2

    while sent < trials:
        sent += 1
        if world_rank == sent % 2:
            comm.bsend(data, dest=partner_rank, tag=0)
        else:
            _ = comm.recv(source=partner_rank, status=None, tag=0)

    return


def write_to_file(write_to, data):
    with open("./{}".format(write_to), "w+") as fp:
        for i in range(len(data)):
            fp.write(str(data[i]) + "\n")


if __name__ == '__main__':
    comm = MPI.COMM_WORLD
    world_rank = comm.Get_rank()

    if world_rank == 0:
        MPI.Attach_buffer(np.zeros(MPI.BSEND_OVERHEAD * 10))
    comm.Barrier()
    if world_rank == 1:
        MPI.Attach_buffer(np.zeros(MPI.BSEND_OVERHEAD * 10))
    world_rank = comm.Get_rank()
    max_char_count = 50000000  # 50MB
    step = 10000
    data_sending_duration = np.zeros(
        int(max_char_count / step), dtype=np.double)
    trials = 1000

    for i, d_size in enumerate(range(1, max_char_count, step)):
        elapsed_time = MPI.Wtime()
        send_data(d_size, trials)
        data_sending_duration[i -
                              1] = (MPI.Wtime() - elapsed_time) / trials / 2

    if world_rank == 0:
        write_to = sys.argv[1]
        write_to_file(write_to, data_sending_duration)

    MPI.Finalize()
