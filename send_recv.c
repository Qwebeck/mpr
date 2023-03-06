#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


void send_data(int char_count, int trials) {
  char* data;
  data = (char*) malloc(char_count * sizeof(char));
  unsigned long i = 0;
  for (i = 0; i < char_count; ++i) {
    data[i] = '1';
  }

  int sent = 0;
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int partner_rank = (world_rank + 1) % 2;
  while (sent < trials) {
    ++sent;
    if (world_rank == sent % 2) {
      MPI_Send(
        data, 
        char_count, 
        MPI_BYTE, 
        partner_rank, 
        0,
        MPI_COMM_WORLD
      );
    } else {
      MPI_Recv(
        data, 
        char_count, 
        MPI_BYTE, 
        partner_rank, 
        0,
        MPI_COMM_WORLD,
        MPI_STATUS_IGNORE
      );
    }
    free(data);
  }
}



void store_to_file(double* data, unsigned long size) {
    /*
    output_one_node_MPI_Send
    output_two_nodes_MPI_Send
    output_one_node_MPI_RSend
    output_two_nodes_MPI_RSend
    
    */
    FILE *fp = fopen("/home/students/f/o/forostia/sem1/tpr_lab/output_one_node_MPI_Send.txt", "w+");
    unsigned long i;
    for (i = 0; i < size; i++) {
        fprintf(fp, "%f\n", data[i]);
    }
    fclose(fp);
}


int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);
  double elapsed_time = 0;
  unsigned long max_char_count = 50000000; // 500MB
  double* data_sending_duration;
  data_sending_duration = (double*) malloc(max_char_count * sizeof(double));
  int trials = 1000;
  MPI_Barrier(MPI_COMM_WORLD);
  unsigned long i;
  for (i = 1; i < max_char_count; ++i) {
    elapsed_time = MPI_Wtime();
    send_data(i, trials);
    data_sending_duration[i - 1] = (MPI_Wtime() - elapsed_time) / trials / 2;
  }
  
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  // if (world_rank == 0) {
  //   store_to_file(data_sending_duration, max_char_count);
  // }

  free(data_sending_duration);
  MPI_Finalize();
  return 0;
}