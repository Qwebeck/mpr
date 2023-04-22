#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

/**
Autorzy:
Bohdan Forostianyi
Filip Księżyc
*/

int compare(const void *a, const void *b)
{
    int int_a = *((int *)a);
    int int_b = *((int *)b);

    if (int_a == int_b)
        return 0;
    else if (int_a < int_b)
        return -1;
    else
        return 1;
}

void append(int *data, int el)
{
    int i = 0;
    while (data[i] != -1)
    {
        i++;
    }
    data[i] = el;
}

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

double *initialize_empty_array(int size)
{
    double *array = malloc(size * sizeof(double));
    int i;
    for (i = 0; i < size; i++)
    {
        array[i] = -1;
    }
    return array;
}

int main(int argc, char *argv[])
{
    int i, j;
    volatile double random;
    double start, stop;
    unsigned short tab[3];
    int table_size = 10000;

    double *to_sort = initialize_empty_array(table_size);
    int bucket_size = 100;
    // int buckets[table_size / bucket_size][bucket_size];
    // int bucket_position;

    /** Time variables */

    start = omp_get_wtime();
    for (i = 0; i < table_size; i++)
    {
        random = erand48(tab);
        to_sort[i] = random;
    }
    stop = omp_get_wtime();
    save_to_file("erand48_distribution", to_sort, table_size);
    free(to_sort);
    return 0;
}
