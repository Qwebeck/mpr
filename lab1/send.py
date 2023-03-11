#!/usr/bin/python
from mpi4py import MPI
import numpy as np


def send_data(char_count, trials):
    data = np.ones(char_count, dtype=np.byte)
    sent = 0
    world_rank = comm.Get_rank()
    partner_rank = (world_rank + 1) % 2

    while sent < trials:
        sent += 1
        if world_rank == sent % 2:
            comm.send(data, dest=partner_rank, tag=0)
        else:
            _ = comm.recv(source=partner_rank, status=None, tag=0)

    return


def store_to_file(data):
    """
    output_one_node_MPI_Send
    output_two_nodes_MPI_Send
    output_one_node_MPI_RSend
    output_two_nodes_MPI_RSend
    """
    with open("./output_one_node_MPI_send.txt", "w+") as fp:
        for i in range(len(data)):
            fp.write(str(data[i]) + "\n")


comm = MPI.COMM_WORLD
world_rank = comm.Get_rank()
max_char_count = 50000000  # 500MB
step = 10000
data_sending_duration = np.zeros(max_char_count / step, dtype=np.double)
trials = 1000

for i, d_size in enumerate(range(1, max_char_count, step)):
    elapsed_time = MPI.Wtime()
    send_data(d_size, trials)
    data_sending_duration[i - 1] = (MPI.Wtime() - elapsed_time) / trials / 2

if world_rank == 0:
    store_to_file(data_sending_duration)



MPI.Finalize()
