/*
 * main.c
 *
 *  Created on: Aug 17, 2020
 *      Author: gai
 */

/* calculate M given matrix A - A is a spmat  */
#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "Division.h"
#include "Matrix.h"
#include "Modularity.h"
#include "PowerIteration.h"
#include "CheckingAlgorithems.h"



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

			AddRow(matrix , mask, k, i);
		}
	}

	createNmatrix(matrix);
	calculateMatrixNorm(matrix);


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







