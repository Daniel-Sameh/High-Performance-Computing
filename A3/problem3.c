#include <stdio.h>
#include "mpi.h"
#include <omp.h>
#include <stdlib.h>

#define ll long long

int main(int argc, char*argv[]){
    int p, my_rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    const int n = 10000000;
    int* arr;
    ll *process_sum;

    int size = n / p;

    ll local_sum = 0;

    if (my_rank == 0) {
        arr = malloc((n + 1) * sizeof(int));

        #pragma omp parallel for
        for (int i = 0; i <= n; ++i) {
            arr[i] = i;
        }

        #pragma omp parallel for reduction(+:local_sum)
        for (int i = p * size; i <= n; ++i) {
            local_sum += arr[i];
        }

        process_sum = malloc(p * sizeof(ll));
    }

    int* process_arr = malloc(size * sizeof(int));

    MPI_Scatter(arr, size, MPI_INT, process_arr, size, MPI_INT, 0, MPI_COMM_WORLD);

    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < size; ++i) {
        local_sum += process_arr[i];
    }


    MPI_Gather(&local_sum, 1, MPI_LONG_LONG, process_sum, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        ll total_sum = 0;

        #pragma omp parallel for reduction(+:total_sum)
        for (int i = 0; i < p; ++i) {
            total_sum += process_sum[i];
        }

        printf("Total array sum = %lld\n", total_sum);
    }

    MPI_Finalize();

}
