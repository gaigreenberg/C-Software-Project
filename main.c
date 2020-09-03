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

/*error detect */
void readErrCheck(int actual, int expected, int errorNum){
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

void checkM(int M){
	if( M == 0){
		printf("M=0 ; there are no edges in the graph");
		exit(EXIT_FAILURE);
	}
}

void forceStop( const char* func,const int line){
	printf("\n ~ forced to stop in %s, line %d", func, line);
	exit(EXIT_SUCCESS);
}

/*claculate M of matrix A when M is the sum of vertices degrees */
double calculateM(spmat* A ,int n ){
	int i;
	double count = 0 ;
	for(i=0 ; i<n ; ++i ){
		count +=(double) A->Kvec[i];
	}
	return count;

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
void loadMatrix(FILE* input, spmat* matrix, int n){

	int i, r, k, *mask = (int*)calloc(n,sizeof(int));

	rewind(input);
	r = fread(&n, sizeof(int), 1, input);
	/*printf("|V| = %d\n",n);*/
	(readErrCheck(r,1,1));		/*errror 1*/

	for (i=0; i<n; i++){
		r = fread(&k, sizeof(int), 1, input); /*read Ki*/
		readErrCheck(r, 1, 2); /*error 2*/
		(matrix->Kvec)[i]=k;

		if(k>0){
			r = fread(mask, sizeof(int), k, input);
			readErrCheck(r, k, 3); /*error 3*/

			AddRow2(matrix , mask, k, i);
		}
	}
	free(mask);

}

/* write partiton with maximized modularity*/
void writeDivision(FILE* output, division* div){
	int k, n=div->size;
	int* members;
	groupCell *current = div->groups, *prev;

	fwrite(&n, sizeof(int), 1 , output);

	while(current != NULL){
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
int Alogrithem2(spmat* A, spmat* Bg,int M, int* subDivision, int* g, int n, int* a, int* b){
	double value, *vec, Q, *colSum;
	int j;

	*a=0 ; *b=0;
	vec 		  = (double*)calloc(n, sizeof(double));
	subDivision   = (int*)calloc(n,sizeof(int));
	colSum		  = calloc(A->n, sizeof(double));
	printf("A2: calculating Bg\t-> ");

	createBg(A,M,g,Bg,colSum);


	printf("\ncalculating Bg fucking Hat\t-> ");
	createBgHat(Bg, colSum);

	printf("calculating eigen pair \t-> ");
	value = calculateEigenPair(vec, Bg, n, colSum);

	if (value <=0){
		return 0;
	}
	printf("creating sub-Division vector\t-> ");
	for(j=0; j<n ; j++){
		if(vec[j]<0){
			subDivision[j] = -1;
			++(*a);
		}else if(vec[j]>0){
			subDivision[j] = 1;
			++(*b);
		}

	}

	printf("calculating delta fucking Q\t ");
	Q = calculateDeltaQ(subDivision, Bg);
	if(Q < 0){
		return 0;
	}
	printf("A2;\n");

	free(vec);
	return 1;
}

/*B should be created before, divisions O&P should be created before before*/
void Alogrithem3(spmat* A, spmat* Bg, int n,int M, division* O, division* P){
	group *X = calloc(1,sizeof(group)), *Y=calloc(1,sizeof(group));
	int *g = calloc(n, sizeof(int)), *subDiv = calloc(n,sizeof(int));
	int a=0 ,b=0, divideable;

	printf("\t A3: Entering loop:\n");

	while (P->size > 0){
		removeG(P, g); /* what to doo?? */;
		divideable = Alogrithem2(A, Bg, M, subDiv,g,n,&a,&b );

		if(divideable){
			printf("\t\tA3:dividavle\n");
			subDividedBySubdiviosion(X,Y, subDiv,n, a, b);
			reOrder(P,O,X,Y);
		}
		else{
			printf("\t\tA3:un-dividavle\n");

			createDivision(X, n, g);
			add(O,X);
		}
		free(Bg);
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
	spmat *A, *Bg;
	division *O = allocateDivision, *P = allocateDivision /*, *test=allocateDivision*/;
	int n;
	double M;

	(checkArgc(argc));
	srand(time(NULL));
	inMatrix  = fopen(argv[1],"r");
	outputDiv = fopen(argv[2],"w");

	/*read input matrix to sparse and store as Sparse Matrix */
	fread(&n, 1 ,sizeof(int), inMatrix);
	A = allocateMatrix(n);

	loadMatrix(inMatrix,A,n);
	printMatrix(A);
	printf("\n\t ~~ loading Graph Complete ~~\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	/* pre calculations & settings */
	Bg  = allocateMatrix(n);
	M  = calculateM(A,n);
	printf("M = %f\n", M);
	checkM(M);

	setEmptyDivision(O);
	setTrivialDivision(P,n);


	/*works untill here! without B-part*/
	/* main calculation and outputting*/

	Alogrithem3(A, Bg, n, M, O, P);
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







