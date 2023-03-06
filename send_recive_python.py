#!/usr/bin/python
from mpi4py import MPI
import numpy as np

def send_data(char_count, trials):
    data = np.ones(char_count + MPI.BSEND_OVERHEAD, dtype=np.byte)
    sent = 0
    world_rank = comm.Get_rank()
    partner_rank = (world_rank + 1) % 2

    while sent < trials:
        sent += 1
        if world_rank == sent % 2:
            comm.Bsend(data, dest=partner_rank, tag=0)
        else:
            comm.recv(data, source=partner_rank, status=None, tag=0)

    return


def store_to_file(data, size):
    """
    output_one_node_MPI_Send
    output_two_nodes_MPI_Send
    output_one_node_MPI_RSend
    output_two_nodes_MPI_RSend
    """
    with open("/home/students/f/o/forostia/sem1/tpr_lab/output_two_nodes_MPI_BSend.txt", "w+") as fp:
        for i in range(size):
            fp.write(str(data[i]) + "\n")


comm = MPI.COMM_WORLD
world_rank = comm.Get_rank()
max_char_count = 5000  # 500MB
data_sending_duration = np.zeros(max_char_count, dtype=np.double)
trials = 1000

for i in range(1, max_char_count):
    elapsed_time = MPI.Wtime()
    send_data(i, trials)
    data_sending_duration[i - 1] = (MPI.Wtime() - elapsed_time) / trials / 2

if world_rank == 0:
    store_to_file(data_sending_duration, max_char_count)

MPI.Finalize()
