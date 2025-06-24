#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    int n;
    printf("Enter n: ");
    scanf("%d", &n);

    int** mat = malloc(n * sizeof(int *));

    printf("Enter the matrix: \n");
    for (int i = 0; i < n; ++i) {
        mat[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j) {
            scanf("%d", &mat[i][j]);
        }
    }

    printf("Enter the vector: \n");

    int* v = malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        scanf("%d", &v[i]);
    }

    int* ret = malloc(n * sizeof(int));

    #pragma omp parallel shared(mat, v, ret)
    {
        #pragma omp for
        for (int i = 0; i < n; ++i) {
            ret[i] = 0;
        }

        #pragma omp for
        for (int row = 0; row < n; ++row) {
            for (int i = 0; i < n; ++i) {
                ret[row] += mat[row][i] * v[row];
            }
        }
    }

    printf("Resulting Vector:\n");
    for (int i = 0; i < n; ++i) {
        printf("%d ", ret[i]);
    }
    printf("\n");

    return 0;
}
