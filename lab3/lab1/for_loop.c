#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

/**
Autorzy:
Bohdan Forostianyi
Filip Księżyc
*/

void save_to_file(char *filename, double *data, int size)
{
    FILE *fp;
    int i;
    fp = fopen(filename, "w");
    for (i = 0; i < size; i++)
    {
        fprintf(fp, "%F\n", data[i]);
    }
    fclose(fp);
}

int main(int argc, char *argv[])
{
    int i, j;
    volatile int random;
    double start;
    double stop;
    int times_n = 5;
    double time[times_n];
    unsigned short tab[3];
    int table_size = 10000000;
    int random_numbers[table_size];

    for (j = 1; j < times_n + 1; j++)
    {
        start = omp_get_wtime();
#pragma omp parallel for schedule(static) private(i, tab, random) num_threads(3)
        for (i = 0; i < table_size / j; i++)
        {
            random = erand48(tab);
            random_numbers[i] = random;
        }
        stop = omp_get_wtime();
        time[j - 1] = stop - start;
    }

    save_to_file("for_loop.txt", time, times_n);
    // for (i = 0; i < 4; i++)
    //     printf("Thread number = %d, Time = %F\n", i + 1, time[i]);

    return 0;
}
