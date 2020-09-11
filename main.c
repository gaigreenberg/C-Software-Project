/*
 * main.c
 *
 *  Created on: Aug 17, 2020
 *      Author: gai
 */

/* calculate M given matrix A - A is a spmat  */
#include "division.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "Matrix.h"
#include "Modularity.h"

#define allocateDivision ((division*)calloc(1,sizeof(division)))
#define GP(A,B) {printf("\n");printGroup(A,"A");printf("\n");printGroup(B,"B");printf("\n");}

/*error detect */
void REC(int actual, int expected, int errorNum){
	if(actual != (expected)){
		printf("error in loadMatrix #%d \t",errorNum);
		printf("actual = %d, expected =%d \n ", actual, expected);
		exit(EXIT_FAILURE);
	}
}

void checkArgc(int argc){
	if (argc != 3){
		exit(EXIT_FAILURE);
	}
}

/*
 * forceStop(__FUNCTION__ , __LINE__)
 */
void forceStop( const char* func,const int line){
	printf("\n ~ forced to stop in %s, line %d", func, line);
	exit(EXIT_SUCCESS);
}




/* update row according to mask */
void updateRow(double* row, int n, int* mask, int k){
	int i, nbr;

	for (i=0; i<n; i++){
		row[i]=0.0;
	}
	if(k>n){printf("Fatal Readinf Error"); exit(EXIT_FAILURE);}
	for (i=0; i<k; i++){
		nbr = mask[i];
		row[nbr] = 1.0;
	}
}

/* read input and store in matrix, after allocation
 * closes input file*/
void loadMatrix(FILE* input, Matrix* matrix, int n){

	int i, r, k, *mask = (int*)calloc(n,sizeof(int));

	rewind(input);
	r = fread(&n, sizeof(int), 1, input);
	/*printf("|V| = %d\n",n);*/
	(REC(r,1,1));		/*errror 1*/

	for (i=0; i<n; i++){
		r = fread(&k, sizeof(int), 1, input); /*read Ki*/
		REC(r, 1, 2); /*error 2*/

		(matrix->K)[i]=k;
		matrix->M +=k;

		if(k>0){
			r = fread(mask, sizeof(int), k, input);
			REC(r, k, 3); /*error 3*/

			addRowNewFormat(matrix , mask, k, i);
		}
	}

	createNmatrix(matrix);
	calculateNorm1(matrix);


	free(mask);

}

/* write partiton with maximized modularity*/
void writeDivision(FILE* output, division* div){
	int k, n=div->DivisionSize;
	groupCell *current = div->groups, *prev;

	fwrite(&n, sizeof(int), 1 , output);

	while(current != NULL){
		k = current->groupSize;
		fwrite(&k, sizeof(int), 1, output);
		fwrite((current->group), sizeof(int), k, output);
		prev = current;
		current = current->nextGroup;
		freeGroupCell(prev);

	}
	fclose(output);

}

/*B should be created before, Bg,subDivision should be allocated before*/
int Alogrithem2(Matrix* matrix, int* subDiv, int* g, int n, int* a, int* b){
	double value, *vec, Q;
	int j;

	matrix -> filter = g;

	*a=0 ; *b=0;
	vec = (double*)calloc(n, sizeof(double));

	/*createBg(matrix,g,Bg,colSum);
	createBgHat(Bg, colSum);
	 */
	value = calculateEigenPair(matrix, vec, n);

	if (value <=0){
		return 0;
	}
	printf("in Alg2: ");
	printVector(vec,n,0);
	for(j=0; j<n ; j++){
		if(vec[j]<0){
			subDiv[j] = -1;

			++(*a);
		}else if(vec[j] > 0){
			subDiv[j] = 1;
			++(*b);
		}

	}
	Q = calculateDeltaQ(subDiv, matrix);
	if(Q < 0){
		return 0;
	}

	free(vec);
	return 1;
}

/*B should be created before, divisions O&P should be created before before*/
void Alogrithem3(Matrix* matrix, int n, division* O, division* P){
	groupCell *A = cg, *B=cg;
	int *g = (int*)calloc(n, sizeof(int)), *subDiv = (int*)calloc(n,sizeof(int));

	int a=0 ,b=0, divideable , breaker=0;

	/*printf("\nA3: Entering loop:\n");*/

	while (P->DivisionSize > 0){
		removeG(P, g); /* what to doo?? */;
		divideable = Alogrithem2(matrix, subDiv,g,n,&a,&b );
		printf("after Alg2: ");
		printIntVector(subDiv,n,100);
		if(divideable){
			breaker++;
			printf("dividavle\t");
			subDividedBySubdiviosion(A,B, subDiv,n, a, b);

			GP(A,B);
			printf("\n");
			reOrder(P,O,A,B);
			if(breaker == 25){
				printf("Algorithem 3 got stuck in LOOP");
				forceStop(__FUNCTION__, __LINE__);
			}
		}
		else{
			printf("\n->reached un-dividavle");

			CreateGroupCell(A, n, g);
			add(O,A);
		}
	}
}

/*side functions for tests */

/*prints output file*/
void printDivisiobFile(char *argv2){
	FILE* out = fopen(argv2,"r");
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

/* argv[1] = input || argv[2] = output */
int main(int argc, char* argv[]) {
	FILE *inMatrix, *outputDiv;
	Matrix *matrix;
	division *O = allocateDivision, *P = allocateDivision /*, *test=allocateDivision*/;
	int n;

	(checkArgc(argc));
	srand(time(NULL));
	inMatrix  = fopen(argv[1],"r");
	outputDiv = fopen(argv[2],"w");

	/*read input matrix to sparse and store as Sparse Matrix */
	fread(&n, 1 ,sizeof(int), inMatrix);
	matrix = allocateMatrix(n);

	loadMatrix(inMatrix,matrix,n);
	printf("\n\t ~~ loading Graph Complete ~~\n");



	setEmptyDivision(O);
	setTrivialDivision(P,n);


	/*works untill here! without B-part*/
	/* main calculation and outputting*/

	Alogrithem3(matrix, n, O, P);
	printf("\n Algorithem 3 is Done\n_________________________________________________________________________________________________________\n");

	forceStop(__FUNCTION__, __LINE__);
	writeDivision(outputDiv,O);

	/*free memory and finish program
	free(inMatrix);
	free(outputDiv);
	freeDivision(O);
	freeDivision(P);
	freeMatrix(Bg);
	freeMatrix(A);*/
	printf("\n\n ~~~ ~~ ~ finished running successfully ~ ~~ ~~~");
	return 0;
}







