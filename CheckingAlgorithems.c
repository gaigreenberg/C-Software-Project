/*
 * CheckingAlgorithems.c
 *
 *  Created on: 11 ����� 2020
 *      Author: omlad
 */

#include "CheckingAlgorithems.h"

/*
 * forceStop(__FUNCTION__ , __LINE__)
 */
void forceStop( const char* func,const int line){
	printf("\n ~ forced to stop in %s, line %d", func, line);
	exit(EXIT_SUCCESS);
}

/*check if argc == 3*/
void checkArgc(int argc){
	if (argc != 3){
		exit(EXIT_FAILURE);
	}
}

/*check if fread & fwrite finished as expected*/
void REC(int actual, int expected, int errorNum){
	if(actual != (expected)){
		printf("error in loadMatrix #%d \t",errorNum);
		printf("actual = %d, expected =%d \n ", actual, expected);
		exit(EXIT_FAILURE);
	}
}

/*visualise information*/

/*print douvle vector*/
void printVector(double* vector, int n,char* name){
	int j;
	printf("%s=\n\t{", name);
	for(j=0; j<n ; j++){
			printf("%.2f", vector[j]);
			if(j<(n-1)) printf(", ");
	}
	printf("}\n");
}

/*print int vector*/
void printIntVector(int* vector, int n,char* name){
	int j;
	printf("%s=\n{", name);
	for(j=0; j<n ; j++){
			printf("%d", vector[j]);
			if(j<(n-1)) printf(", ");
	}
	printf("}\n");
}


/*prints input file*/
void printGraph(FILE* input, int n){
	int i,k,j, *mask = (int*)calloc(n,sizeof(int));

	rewind(input);
	fread(&n, sizeof(int), 1, input);
	printf("|V| = %d",n);

	for (i=0; i<n; i++){
		fread(&k, sizeof(int), 1, input); /*read Ki*/
		printf("\n\t  K%d = %d ->\t", i, k);
		fread(mask, sizeof(int), k, input);
		if(k>0){
			for(j=0;j<k;j++){
				printf("%d,",mask[j]);
			}
		}
	}
	free(mask);
}

/*prints output file - shpuld be completly written before*/
void printDivisionFile(char *outPutFile){
FILE* out = fopen(outPutFile,"r");
int n, j, i, d, *indices;
fread(&n, sizeof(int), 1, out);
indices = (int*)calloc(n,sizeof(int));
printf("there are %d groups in the division:",n);
for(j=0; j<n;j++){
	fread(&d,sizeof(int),1,out);
	fread(indices,sizeof(int),d,out);
	printf("\ngroup #%d has %d nodes:\n\t ",j+1,d);
	for(i=0; i<d; i++){
		printf("%d,",indices[i]);
	}
}


}

