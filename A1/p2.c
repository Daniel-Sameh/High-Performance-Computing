#include <stdio.h>
#include "mpi.h"
#include <string.h>
#include <stdlib.h>
int main(int argc, char*argv[]){
	int p, rank;
	char state ;
	char method;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(argc>1 && strlen(argv[1])>0){
		state= argv[1][0];
	}
	else{
		if(rank==0){
			printf("Please Add The State.\n");

		}
		return 0;
     
	}
	if (argc > 2 && strlen(argv[2]) > 0) {
		method = argv[2][0];
	} else {
		if (rank == 0) {
			printf("Please Add The Input method Type.\n");

		}
		return 0;
			
	}
	if(state!='e' && state!='d'){
		if (rank == 0) {
			printf("'%c' is not acceptable\nPlease enter a valid state either 'e' for encryption or 'd' for decryption\n",state);
		}
		return 0;
       
	}
	if(method!='c' && method!='f'){
		if (rank == 0) {
			printf("'%c' is not acceptable\nPlease enter a valid input method either 'c' for cmd or 'f' for file\n",method);
		}
		return 0;
	}
	int size;
	int rem;
	char a[100];
	if(rank==0){

		if(method=='c'){
			printf("Input: ");
			fflush(stdout);
			scanf("%s",a);
			size=strlen(a)/p;
			rem=strlen(a)%p;
		}
		else{
			printf("Input file: ");
			fflush(stdout);
			char filex[100];
			scanf("%s",filex);
			FILE *file = fopen(filex, "r");
			if (file == NULL) {
				printf("\nCan't find the selected file.\n\n");
				return 0;
			}
			fgets(a, sizeof(a), file);
			fclose(file);
			size_t len = strlen(a);
			if (a[len - 1] == '\n') {
				a[len - 1] = '\0';
			}

			size=strlen(a)/p;
		  	rem=strlen(a)%p;

		}
	}
	int sign=-1;
	if(state=='e'){
		sign=1;
	}
	if(rank==0){
		for (int i = 1; i < p; i++){
			int sendsize=size+(i<rem?1:0);// law el remainder 3 masalan wi el size 2 yeb2a awel 3 rocesses haya5do char zeyada yeb2a kol wa7ed haya5od 3
			MPI_Send(&sendsize,1,MPI_INT,i,0,MPI_COMM_WORLD);
			MPI_Send(&a[(i)*size+(i<rem?i:rem)], sendsize, MPI_CHAR, i, 0, MPI_COMM_WORLD);// bazawed 3ala el index el zyada elly bsbb elremainder awel lama el i teb2a 2ad el reaminder bazawed el remainder 3alatol

		}
		int my_size = size + (0 < rem ? 1 : 0);
		for (int i = 0; i < my_size; i++){
			a[i]='a'+(a[i]-'a'+3*sign+26)%26;// ba3ml kda masaln 34an law z wi encryption yeb2a c wi law a wi decryption masalan teb2a x 34an kda el 26
		}
		for (int i = 1; i < p; i++){
			int recv_size = size + (i < rem ? 1 : 0);
			MPI_Recv(&a[(i)*size+(i<rem?i:rem)], recv_size, MPI_CHAR, i, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}

	}else{
		int size;
		MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		char local[size];
		MPI_Recv(local, size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for(int i=0; i<size; i++){
			local[i]='a'+(local[i]-'a'+3*sign+26)%26;
		}
		MPI_Send(local, size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}

	if(rank==0){
		if(method=='c'){
			printf("Output: ");

			for (int i = 0; i < strlen(a); i++){
				printf("%c", a[i]);
			}
			printf("\n");
		}
		else{
			FILE *outFile = fopen("output.txt", "w");
			if (outFile == NULL) {
				printf("Failed to write in the file\n");
				return 0;
			}
			for (int i = 0; i < strlen(a); i++) {
				fprintf(outFile, "%c", a[i]);
			}
			fprintf(outFile, "\n");
			fclose(outFile);
		}
	}
	MPI_Finalize();

}