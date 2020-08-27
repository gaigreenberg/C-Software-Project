/*
 * main.c
 *
 *  Created on: Aug 17, 2020
 *      Author: gai
 */

/* calculate M given matrix A - A is a spmat  */
#include "spmat.h"
#include "Bmat.h"
#include <assert.h>
#include <stdio.h>
#include <stddef.h>


int calculateM(spmat* A ,int n ){
	int i , count = 0 ;
	for(i=0 ; i<n ; ++i ){
		count += A->Kvec[i];
	}
	return count;

}

void updateRow(int* row, int n, int* mask, int k){
	int i, neighbor, j=0;

	if (k == 0){
		for (i=0; i<n; i++){
			row[i]=0;
		}
		return;
	}
	neighbor = mask[0];
	for (i=0; i<n; i++){
		if (i==neighbor && j<k){
			row[i] = 1;
			j++;
			neighbor = mask[j];

		}else{

		row[i]=0;
		}
	}
}

/* read input and store in matrix, after allocation*/
void loadMatrix(FILE* input, spmat* matrix, int n){
	int i,r,k;
	int *row, *mask;

	rewind(input);
	r = fread(&n, 1, sizeof(int), input);
	assert(r==1);
	row =  (int*)calloc(n, sizeof(int));
	mask = (int*)calloc(n, sizeof(int));
	for (i=0; i<n; i++){
		fread(&k, 1 ,sizeof(int) , input);
		matrix->Kvec[i]=k;
		updateRow(row,n,mask,k);
		AddRow(matrix , row, i);
	}
	free(row);
	free(mask);
}

/* write partiton with maximized modularity*/
void writePartition(FILE* output, int n){

}

/*B should be created before, Bg should be allocated before*/
int Alogrithem2(spmat* B, spmat* Bg, int* g, int n){
	double value, *vec;
	int j,*s,Q;
	vec = (double*)calloc(n, sizeof(double));
	s   = (int*)calloc(n,sizeof(int));
	createBg(B,g,Bg);
	createBgHat(Bg);

	value = calculateEigenPair(vec, Bg ,n);

	if (value <=0){
		return 0;
	}
	for(j=0; j<n ; j++){
		if(vec[j]<=0){
			s[j] = -1;
		}else{
			s[j] = 1;
		}

	}
	Q = calculateQ(s, B);
	if(Q < 0){
		return 0;
	}
	for (j=0; j<n; j++){
		g[j] = g[j] * s[j];
	}

	free(vec);
	free(s);
	return 1;
}



/* argv[1] = input || argv[2] = output */
int main(int argc, char* argv[]) {
	FILE *inMatrix, *outMatrix;
	spmat *A , *B, *C;
	int n;

	if(argc != 3){
		printf("invalid number of arguments!");
		return(1);
	}
	inMatrix  = fopen(argv[1],"r");
	outMatrix = fopen(argv[2],"w");
	fread(&n, 1 ,sizeof(int), inMatrix);
	A = allocateMatrix(n); B = allocateMatrix(n) ; C = allocateMatrix(n);
	loadMatrix(inMatrix, A, n);


	/* BODY */


	writePartition(outMatrix, n);

	return 0;
}







