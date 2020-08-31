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
	Q = calculateDeltaQ(subDivision, B);
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
	spmat *A, *B;
	division *O = allocateDivision, *P = allocateDivision, *test=allocateDivision;
	int n, M;

	(checkArgc(argc));

	inMatrix  = fopen(argv[1],"r");
	outputDiv = fopen(argv[2],"w");

	/*read input matrix to sparse and store as Sparse Matrix */
	fread(&n, 1 ,sizeof(int), inMatrix);
	printf("n=%d \n\n",n);
	A = allocateMatrix(n);

	loadMatrix(inMatrix,A,n);
	printf("\n\t ~~ loading Graph Complete ~~\n");


	/* pre calculations */
	B  = allocateMatrix(n);
	M  = calculateM(A,n);
	printf("M calculated:%d\n",M);

	/*createB(A,M,B);*/


	/* main calculation and outputting*/
	setEmptyDivision(O);
	setTrivialDivision(P,n);
	printf("trivial & empty divisions created");
	if(1){

		/*test division writing*/
		madeUpDivision(test);
		writeDivision(outputDiv, test);
		/*fclose(outputDiv);*/
		printDivisiobFile(argv[2]);
		exit(EXIT_SUCCESS);
	}

	/*works untill here! without B-part*/
	Alogrithem3(B, n, O, P);
	writeDivision(outputDiv,O);

	/*free memory and finish program */
	fclose(outputDiv);
	free(inMatrix);
	free(outputDiv);
	freeDivision(O);
	freeDivision(P);
	freeMatrix(B);
	freeMatrix(A);
	return 0;
}







