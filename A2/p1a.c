#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, subrange, remainder, lower_bound = 0, upper_bound = 0, count = 0, result = 0;
    double start, end, total_time = 0;
    MPI_Init(NULL, NULL);
    
  
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0){
        printf("please enter the lower bound number: ");
        fflush(stdout);
        scanf("%i",&lower_bound);

        printf("please enter the upper bound number: ");
        fflush(stdout);
        scanf("%i",&upper_bound);
        start = MPI_Wtime();

        subrange = (upper_bound - lower_bound + 1) / (size - 1);
        remainder = (upper_bound - lower_bound + 1) % (size - 1);
        // end = MPI_Wtime();
        // total_time += end - start;
    }

    MPI_Bcast(&lower_bound, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&subrange, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&remainder, 1, MPI_INT, 0, MPI_COMM_WORLD);


    // for(int i = 1; i < size; ++i){
    int first_in_chunk = (rank - 1) * subrange + lower_bound;
    int last_in_chunk = first_in_chunk + subrange - 1;
    int *ptr_to_last_in_chunk = &last_in_chunk;

    if(rank != 0){
        if(remainder != 0 && rank == size - 1){
            *ptr_to_last_in_chunk += remainder;
            // printf("-process number #%i recieved the chunck from %i to %i\n", rank, first_in_chunk, last_in_chunk);
            // fflush(stdout);
        }
        //else{
            // printf("-process number #%i recieved the chunck from %i to %i\n", rank, first_in_chunk, last_in_chunk );
            // fflush(stdout);
        //}
        // printf("    prime numbers in this chunk are: ");
        fflush(stdout);
        for (int i = first_in_chunk; i <= last_in_chunk; i++)
        {
            if(i < 2)
                continue;
            int is_prime = 1;
            // int prime_exists = 1;
            for(int j = 2; j*j <= i; ++j){
                if(i%j == 0){
                    is_prime = 0;
                    break;
                }
            }
            if(is_prime == 1){
                // printf("%i ",i);
                // fflush(stdout);
                count++;
            }
        }
        // if(count == 0){
            // printf("None \n      Number of prime numbers in this chunk is: 0\n");
            // fflush(stdout);
        // }else{
        //     printf("\n      Number of prime numbers in this chunk is: %i\n", count);
        //     fflush(stdout);
        // }
        printf("Process %d found %d primes in range (%d,%d)\n", rank, count, first_in_chunk, last_in_chunk);

    }

    MPI_Reduce(&count, &result, 1, MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

    if(rank == 0){
        // start = MPI_Wtime();
        // printf("Total number of prime numbers in the given range is: %i\n", result);
        printf("Total number of primes in range (%d,%d) is %d\n", lower_bound, upper_bound, result);
        fflush(stdout);
        // printf("=================================================\n");
        // fflush(stdout);
        end = MPI_Wtime();
        total_time += end - start;
        printf("Process 0 took %f to finish.\n", total_time);
        fflush(stdout);
    }
    
    // end = MPI_Wtime();
    // double total_time = end - start;

    // printf("Process #%i took %f to finish.\n", rank, total_time);
    // fflush(stdout);
    
    MPI_Finalize();
    return 0;
}