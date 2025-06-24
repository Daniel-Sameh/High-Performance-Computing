#include <stdio.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char* argv[]){
	int p, rank;
	double tStart, tEnd;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		printf("Please enter a range(x,y): ");
		fflush(stdout);
		int x, y;
		scanf("%d %d", &x, &y);
		int range= y - x + 1;
		int size = range/(p-1);
		int rem = range%(p-1);

		tStart = MPI_Wtime();


		for (int i = 1; i < p; i++){
			int start = x+(i-1)*size;
			start += (i-1<rem)?(i-1):rem;
			int end = start+size-1;
			// if (i == p - 1) {
			// 	end += rem;
			// }
			if(i-1<rem){
				end += 1;
			}
			MPI_Send(&start, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&end, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
		int totalCnt=0, tmpCnt=0;
		for (int i = 1; i < p; i++){
			MPI_Recv(&tmpCnt, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			totalCnt+=tmpCnt;
		}
		
		tEnd = MPI_Wtime();

		printf("Total number of primes in range (%d,%d) is %d\n", x, y, totalCnt);
		printf("Total processing time: %f seconds\n", tEnd - tStart);

	}else{
		int start, end;
		MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if(start > end) {
			printf("Invalid range received by process %d\n", rank);
			MPI_Finalize();
			return 0;
		}
		int primeCount = 0;
		if(end<2){
			MPI_Send(&primeCount, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			printf("Process %d found %d primes in range (%d,%d)\n", rank, primeCount, start, end);
			MPI_Finalize();
			return 0;
		}
		if(start<2){
			start=2;
		}
		for (int i = start; i <= end; i++) {
			int isPrime = 1;
			for (int j = 2; j*j <= i; j++) {
				if (i % j == 0) {
					isPrime = 0;
					break;
				}
			}
			if (isPrime){
				primeCount++;
			}
			
		}
		printf("Process %d found %d primes in range (%d,%d)\n", rank, primeCount, start, end);
		MPI_Send(&primeCount, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
