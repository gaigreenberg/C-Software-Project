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
 * storing matrix -> {n,A,K,kmFactor,norm}
 * closes input file
 * */
void loadMatrix(FILE* input, Matrix* matrix, int n){

	int i, r, k, *mask = (int*)calloc(n,sizeof(int));

	rewind(input);
	r = fread(&n, sizeof(int), 1, input);
	/*printf("|V| = %d\n",n);*/
	REC(__FUNCTION__, r,1,1);		/*errror 1*/

	for (i=0; i<n; i++){
		r = fread(&k, sizeof(int), 1, input); /*read Ki*/
		REC(__FUNCTION__, r, 1, 2); /*error 2*/

		(matrix->K)[i]=k;
		matrix->M +=k;

		if(k>0){
			r = fread(mask, sizeof(int), k, input);
			REC(__FUNCTION__, r, k, 3); /*error 3*/

			AddRow(matrix , mask, k, i);
		}
	}

	createNmatrix(matrix);
	calculateMatrixNorm(matrix);


	free(mask);

}

/*
 * write best-partiton of the graph (with maximized modularity)
 * stored in div
 * */
void writeDivision(FILE* output, division* div){
	int k, r, n=div->DivisionSize, *members;
	groupCell *current = div->groups, *prev;

	r = fwrite(&n, sizeof(int), 1 , output);
	REC(__FUNCTION__, r,1,1);

	while(current != NULL){
		k = current->groupSize;
		members = current->group;
		r = fwrite(&k, sizeof(int), 1, output);
		REC(__FUNCTION__, r,1,2);
		r = fwrite(members, sizeof(int), k, output);
		REC(__FUNCTION__, r,k,3);
		prev = current;
		current = current->nextGroup;
		freeGroupCell(prev);
	}
	free(div);

}

/* argv[1] = input || argv[2] = output */
int main(int argc, char* argv[]) {
	FILE *inMatrix, *outputDiv;
	Matrix *matrix;
	division *O = allocateDivision, *P = allocateDivision /*, *test=allocateDivision*/;
	clock_t current_time;
	int n;

	current_time = clock();
	(checkArgc(argc));
	srand(time(NULL));
	inMatrix  = fopen(argv[1],"r");
	outputDiv = fopen(argv[2],"w");

	fread(&n, 1 ,sizeof(int), inMatrix);
	matrix = allocateMatrix(n);

	loadMatrix(inMatrix,matrix,n);

	printf("\n>>\tloading Graph Complete [%.2f ms] \n", (double)current_time);

	setEmptyDivision(O);
	setTrivialDivision(P,n);

	Alogrithem3(matrix, n, O, P);
	printf("\n>>\tAlgorithem 3 is Done [%.2f ms] \n", (double)current_time);

	writeDivision(outputDiv,O);

	printf("\n>>\tWriting output is Done [%.2f ms] \n", (double)current_time);

	/*free memory and finish program*/
	freeMatrix(matrix);
	fclose(inMatrix);
	fclose(outputDiv);
	printf("\n>>\tMemory freeing is Done: [%.2f ms] \n", (double)current_time);

	printf("\n\n>>\tchecking output:\n");
	checkOutPut(argv[2], n);

	printf("\n\n>> >>   finished running [%.2fms]   << <<\n\n",(double)current_time);
	return EXIT_SUCCESS;
}







