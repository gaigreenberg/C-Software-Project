/*
 * main.c
 *
 *  Created on: Aug 17, 2020
 *      Author: gai
 */

/* calculate M given matrix A - A is a spmat  */
#include "spmat.h"
#include "Bmat.h"
#include "division.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>

#define allocateDivision (division*)calloc(1,sizeof(division))
#define problem 1
#define OK 0

int readErrOccurred(int actual, int expected, int errorNum){
	if(actual != expected){
		printf("error in loadMatrix #%d",errorNum);
		return problem;
	}
	return 0;
}

int loadFiled(int readRes){
	if(readRes != OK){
		printf("failed loading Matrix A!");
		return problem;
	}
	return OK;

}

/*claculate M of matrix A when M is the sum of vertices degrees */
int calculateM(spmat* A ,int n ){
	int i , count = 0 ;
	for(i=0 ; i<n ; ++i ){
		count += A->Kvec[i];
	}
	return count;

}

/* update row according to mask */
void updateRow(double* row, int n, int* mask, int k){
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

/* read input and store in matrix, after allocation
 * closes input file*/
int loadMatrix(FILE* input, spmat* matrix, int n){
	int i,r,k;
	int *mask;
	double *row;

	rewind(input);
	r = fread(&n, 1, sizeof(int), input);
	if(readErrOccurred(r,1,1)){return problem;}		/*errror 1*/

	row =  (double*)calloc(n, sizeof(int));
	mask = (int*)calloc(n, sizeof(int));
	for (i=0; i<n; i++){
		r = fread(&k, 1 ,sizeof(int) , input);
		if(readErrOccurred(r,1,2)){return problem;} /*error 2*/

		matrix->Kvec[i]=k;
		r = fread(&mask, n , sizeof(int), input);
		if(readErrOccurred(r,n,3)){return problem;} /*error 3*/

		updateRow(row,n,mask,k);
		AddRow(matrix , row, i);
	}
	free(row);
	free(mask);
	fclose(input);
	return OK;

}

/* write partiton with maximized modularity*/
void wrtieDivision(FILE* output, division* div){
	int j, k, n=div->size;
	int* members;
	groupCell *current = div->groups, *prev;

	fwrite(&n, sizeof(int), 1 , output);

	for (j=0;j<n;j++){
		k = groupSize(current);
		members = current->grp->members;
		fwrite(&k, sizeof(int), 1, output);
		fwrite(members, sizeof(int), k, output);
		prev = current;
		current = current->nextGroup;
		freeGroupCell(prev);

	}
	fclose(output);
}

/*B should be created before, Bg,subDivision should be allocated before*/
int Alogrithem2(spmat* B, spmat* Bg, int* subDivision, int* g, int n, int* a, int* b){
	double value, *vec, Q;
	int j;

	*a=0 ; *b=0;
	vec 		  = (double*)calloc(n, sizeof(double));
	subDivision   = (int*)calloc(n,sizeof(int));
	createBg(B,g,Bg);
	createBgHat(Bg);

	value = calculateEigenPair(vec, Bg, n);

	if (value <=0){
		return 0;
	}

	for(j=0; j<n ; j++){
		if(vec[j]<0){
			subDivision[j] = -1;
			++(*a);
		}else if(vec[j]>0){
			subDivision[j] = 1;
			++(*b);
		}

	}
	Q = calculateQ(subDivision, B);
	if(Q < 0){
		return 0;
	}

	free(vec);
	return 1;
}

/*B should be created before, divisions O&P should be created before before*/
void Alogrithem3(spmat* B, int n, division* O, division* P){
	group *X = calloc(1,sizeof(group)), *Y=calloc(1,sizeof(group));
	int *g = calloc(n, sizeof(int)), *subDiv = calloc(n,sizeof(int));
	int a=0 ,b=0, divideable;
	spmat* Bg;

	while (P->size > 0){
		removeG(P, g); /* what to doo?? */;
		Bg = allocateMatrix(n);
		divideable = Alogrithem2(B, Bg, subDiv,g,n,&a,&b );
		if(divideable){
			subDividedBySubdiviosion(X,Y, subDiv,n, a, b);
			reOrder(P,O,X,Y);
		}
		else{
			createDivision(X, n, g);
			add(O,X);
		}
		free(Bg);
	}
}

/* argv[1] = input || argv[2] = output */
int main(int argc, char* argv[]) {
	FILE *inMatrix, *bestDivisiob;
	spmat *A, *B;
	division *O = allocateDivision, *P = allocateDivision;
	int n,M,r;

	if(argc != 3){
		printf("invalid number of arguments!");
		return(1);
	}
	inMatrix  = fopen(argv[1],"r");
	bestDivisiob = fopen(argv[2],"w");

	/*read input matrix to sparse and store as Sparse Matrix */
	fread(&n, 1 ,sizeof(int), inMatrix);
	A = allocateMatrix(n);
	r = loadMatrix(inMatrix, A, n);
	if (loadFiled(r)) {return problem;}
	fclose(inMatrix);
	free(inMatrix);

	/* pre calculations */
	B  = allocateMatrix(n);
	M  = calculateM(A,n);
	createB(A,M,B);
	free(A);

	/* main calculation and outputting*/
	setEmptyDivision(O);
	setTrivialDivision(P,n);
	Alogrithem3(B, n, O, P);
	wrtieDivision(bestDivisiob,O);

	/*free memory and finish program */

	free(bestDivisiob);
	freeDivision(O);
	freeDivision(P);
	freeMatrix(B);
	return 0;
}







