#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define EMPTY 0

/**
Autorzy:
Bohdan Forostianyi
Filip Księżyc
*/
int compare(const void *a, const void *b)
{
    double aa = *((double *)a);
    double bb = *((double *)b);

    if (aa == bb)
        return 0;
    else if (aa < bb)
        return -1;
    else
        return 1;
}

int append(double **data, double el, int arr_size)
{
    int i = 0, j = 0, new_arr_size = arr_size;
    while ((*data)[i] != EMPTY)
    {
        i++;
        if (i == arr_size)
        {
            new_arr_size = arr_size + arr_size * 0.1;
            double *new_arr = realloc(*data, new_arr_size * sizeof(double));
            *data = new_arr;
            for (j = arr_size; j < new_arr_size; j++)
            {
                (*data)[j] = EMPTY;
            }
        }
    }
    (*data)[i] = el;
    return new_arr_size;
}

double *initialize_empty_array(int size)
{
    double *array = malloc(size * sizeof(double));
    int i;
    for (i = 0; i < size; i++)
    {
        array[i] = EMPTY;
    }
    return array;
}

int first_el_idx(double *arr)
{
    int i = 0;
    while (arr[i] == EMPTY)
    {
        i++;
    }
    return i;
}

int is_thread_bucket(int thread_count, int thread_num, int bucket_count, int bucket_num)
{
    int bucket_per_thread = ceil((double)bucket_count / thread_count);
    int bucket_start = bucket_per_thread * thread_num;
    int bucket_end = bucket_start + bucket_per_thread;
    if (bucket_start <= bucket_num && bucket_num < bucket_end)
    {
        return 1;
    }
    return 0;
}

// idxs of times
#define GENERATION 0
#define BUCKET_DISTRIBUTION 1
#define BUCKET_SORT 2
#define BUCKET_JOIN 3
#define TOTAL_TIME 4

double *measure_for_threads(int threads, int bucket_multiplier)
{
    int i, j;
    volatile double random;
    double phase_start, measure_start;
    unsigned short tab[3];

    int table_size = 5e8;
    int bucket_val_range = 50;
    int max_value = table_size;
    double *to_sort = initialize_empty_array(table_size);

    int bucket_count = table_size / bucket_val_range;
    int bucket_size = bucket_val_range * bucket_multiplier;

    double *buckets[bucket_count];
    int bucket_sizes[bucket_count];
    for (i = 0; i < bucket_count; i++)
    {
        buckets[i] = initialize_empty_array(bucket_size);
        bucket_sizes[i] = bucket_size;
    }

    double *times = initialize_empty_array(TOTAL_TIME + 1);
    // generation
    measure_start = omp_get_wtime();
    phase_start = measure_start;
    for (i = 0; i < table_size; i++)
    {
        random = erand48(tab) * max_value;
        to_sort[i] = random;
    }
    times[GENERATION] = omp_get_wtime() - phase_start;

    // bucket distribution
#pragma omp parallel num_threads(threads)                                      \
    shared(buckets, to_sort, bucket_val_range, bucket_sizes, times) private(i) \
        firstprivate(threads, table_size, bucket_count)
    {
#pragma omp barrier
        int distribution_start = omp_get_wtime();

        int thread_num = omp_get_thread_num();
        int offset = thread_num * 10;
        for (i = offset; i < table_size; i++)
        {
            double val = to_sort[i];
            int macthing_bucket = val / bucket_val_range;
            if (!is_thread_bucket(threads, thread_num, bucket_count, macthing_bucket))
            {
                continue;
            }
            int new_arr_size = append(&buckets[macthing_bucket], val, bucket_sizes[macthing_bucket]);
            bucket_sizes[macthing_bucket] = new_arr_size;
        }
        for (i = 0; i < offset; i++)
        {
            double val = to_sort[i];
            int macthing_bucket = val / bucket_val_range;
            if (!is_thread_bucket(threads, thread_num, bucket_count, macthing_bucket))
            {
                continue;
            }
            int new_arr_size = append(&buckets[macthing_bucket], val, bucket_sizes[macthing_bucket]);
            bucket_sizes[macthing_bucket] = new_arr_size;
        }
#pragma omp barrier
        times[BUCKET_DISTRIBUTION] = omp_get_wtime() - distribution_start;
    }

    phase_start = omp_get_wtime();

#pragma omp parallel for num_threads(threads)                  \
    shared(buckets, bucket_val_range, bucket_sizes) private(i) \
        firstprivate(threads, table_size, bucket_count)
    for (i = 0; i < bucket_count; i++)
    {
        int bucket_size = bucket_sizes[i];
        qsort(buckets[i], bucket_size, sizeof(double), compare);
    }

    times[BUCKET_SORT] = omp_get_wtime() - phase_start;

    // bucket join
    phase_start = omp_get_wtime();
    int curr_position = 0;
    for (i = 0; i < bucket_count; i++)
    {
        int start_idx = first_el_idx(buckets[i]);
        int bucket_size = bucket_sizes[i];
        for (j = start_idx; j < bucket_size; j++)
        {
            to_sort[curr_position] = buckets[i][j];
            curr_position++;
        }
    }
    times[BUCKET_JOIN] = omp_get_wtime() - phase_start;
    times[TOTAL_TIME] = omp_get_wtime() - measure_start;

    int is_ok = 1;
    for (i = 0; i < table_size; i++)
    {
        if (to_sort[i] < to_sort[i - 1])
        {
            is_ok = 0;
        }
    }
    if (is_ok)
    {
        printf("ok\n");
    }
    else
    {
        printf("not ok\n");
    }
    free(to_sort);
    for (i = 0; i < bucket_count; ++i)
    {
        free(buckets[i]);
    }

    return times;
}

void save_to_file(char *filename, int size, double *total_time, double *numbers_generation_time, double *bucket_distibution_time, double *bucket_sort_time, double *bucket_join_time)
{
    FILE *fp;
    int i;
    fp = fopen(filename, "w");
    fprintf(fp, "total_time,numbers_generation_time,bucket_distibution_time,bucket_sort_time,bucket_join_time\n");
    for (i = 0; i < size; i++)
    {
        fprintf(fp, "%F,%F,%F,%F,%F\n", total_time[i], numbers_generation_time[i], bucket_distibution_time[i], bucket_sort_time[i], bucket_join_time[i]);
    }
    fclose(fp);
}

int main(int argc, char *argv[])
{
    int i, j;
    int max_threads = 12;
    int num_tries = 4;

    /* Times */
    double *total_time = initialize_empty_array(max_threads);
    double *numbers_generation_time = initialize_empty_array(max_threads);
    double *bucket_distibution_time = initialize_empty_array(max_threads);
    double *bucket_sort_time = initialize_empty_array(max_threads);
    double *bucket_join_time = initialize_empty_array(max_threads);
    /**/

    for (i = 1; i <= max_threads; ++i)
    {
        printf("Starting %d threads\n", i);
        for (j = 0; j < num_tries; ++j)
        {
            double *experiment_times = measure_for_threads(i, 1);
            total_time[i - 1] += experiment_times[TOTAL_TIME] / num_tries;
            numbers_generation_time[i - 1] += experiment_times[GENERATION] / num_tries;
            bucket_distibution_time[i - 1] += experiment_times[BUCKET_DISTRIBUTION] / num_tries;
            bucket_sort_time[i - 1] += experiment_times[BUCKET_SORT] / num_tries;
            bucket_join_time[i - 1] += experiment_times[BUCKET_JOIN] / num_tries;
            free(experiment_times);
            save_to_file("bf_results.csv", max_threads, total_time, numbers_generation_time, bucket_distibution_time, bucket_sort_time, bucket_join_time);
        }
    }
    free(total_time);
    free(numbers_generation_time);
    free(bucket_distibution_time);
    free(bucket_sort_time);
    free(bucket_join_time);

    return 0;
}
