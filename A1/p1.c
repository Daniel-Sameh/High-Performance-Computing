#include <stdio.h>
#include "mpi.h"

int main(int argc, char*argv[]){
    int p, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //int *local;
    int maxNum;
    int maxIdx;
    if(rank==0){
        int slaves=p-1;
        printf("Hello from master process.\nNumber of slave processes is %d\n", slaves);
        printf("Please enter size of array...\n");
        int n;
        scanf("%d", &n);
        printf("Please enter array elements ...\n");
        int a[n];
        for(int i=0; i<n; i++){
            scanf("%d", &a[i]);
        }
        int size=n/slaves;
        for (int i = 1; i < p; i++){
            MPI_Send(&size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&a[(i-1)*size], size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        maxNum=a[0];
        maxIdx=0;
        if(n%slaves){
            int rem=n%slaves;
            int idx=n-rem;
            maxNum=a[idx];
            maxIdx=idx;
            for (int i = idx; i < n; ++i){
                if(a[i]>maxNum){
                    maxNum=a[i];
                    maxIdx=i;
                }
            }
            printf("Hello from the master, Max number in my partition is %d and index is %d.\n", maxNum, maxIdx-(n-rem));
        }
        for (int i = 1; i < p; i++){
            int tmpMx;
            int tmpIdx;
            MPI_Recv(&tmpMx, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&tmpIdx, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if(tmpMx>maxNum){
                maxNum=tmpMx;
                maxIdx=tmpIdx+((i-1)*size);
            }
        }
        printf("Master process announce the final max which is %d and its index is %d.\n", maxNum, maxIdx);
        printf("Thanks for using our program.\n");
    }else{
        int size;
        MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int local[size];
        MPI_Recv(local, size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        maxNum=local[0];
        maxIdx=0;
        for(int i=1; i<size; i++){
            if(local[i]>maxNum){
                maxNum=local[i];
                maxIdx=i;
            }
        }
        printf("Hello from slave#%d Max number in my partition is %d and index is %d.\n", rank, maxNum, maxIdx);
        MPI_Send(&maxNum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&maxIdx, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}