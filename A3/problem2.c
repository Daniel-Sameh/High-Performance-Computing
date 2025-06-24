#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int i;
    int n;
    printf("Enter array size: ");
    scanf("%d", &n);

    int* arr = malloc(n * sizeof(int));

    printf("Enter the array: \n");
    for (int i = 0; i < n; ++i) {
        scanf("%d", &arr[i]);
    }

    int sum = 0;
    double variance = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < n; ++i) {
        sum += arr[i];
    }

    double mean = (double) sum / (double) n;

    #pragma omp parallel for reduction(+:variance)
    for (i = 0; i < n; ++i) {
        variance += (arr[i] - mean) * (arr[i] - mean);
    }

    variance /= (double) n;

    double sd = sqrt(variance);

    printf("Standard Deviation: %f\n", sd);

    return 0;
}
