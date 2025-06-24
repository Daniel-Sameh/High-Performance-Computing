#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int* initializeMat(int n, int m, int consoleInput)
{
    int *mat = (int*)malloc(n * m * sizeof(int));

    if (consoleInput)
    {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                scanf("%i", &mat[i * m + j]);
    }
    return mat;
}


void printMat(int *mat, int n, int m)
{
    for (int i = 0; i < n; i++, printf("\n"))
        for (int j = 0; j < m; j++)
            printf("%d ", mat[i * m + j]);
}

int main(int argc, char** argv)
{
    int *matA, *matB, *matC, n, m, share;
    int *localMatA, *localMatB, *localMatC;
    int rank, size;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        printf("Sizes (N x M)...\n");
        printf("N: "), scanf("%d", &n);
        printf("M: "), scanf("%d", &m);

        if ((n * m) % size)
        {
            printf("Total number of elements need to be divisible by %d. Aborting...", size);
            MPI_Abort(MPI_COMM_WORLD, 0);
            return 0;
        }
        share = n * m / size;

        printf("Elements of first matrix...\n");
        matA = initializeMat(n, m, 1);

        printf("Elements of second matrix...\n");
        matB = initializeMat(n, m, 1);

        matC = initializeMat(n, m, 0);
    }
    MPI_Bcast(&share, 1, MPI_INT, 0, MPI_COMM_WORLD);

    localMatA = initializeMat(share, 1, 0);
    localMatB = initializeMat(share, 1, 0);
    localMatC = initializeMat(share, 1, 0);

    MPI_Scatter(matA, share, MPI_INT, localMatA, share, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(matB, share, MPI_INT, localMatB, share, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < share; i++)
        localMatC[i] = localMatA[i] + localMatB[i];

    MPI_Gather(localMatC, share, MPI_INT, matC, share, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Result: \n");
        printMat(matC, n, m);
    }
    
    MPI_Finalize();
}
