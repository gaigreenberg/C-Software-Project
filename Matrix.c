/*
 * Matrix.c
 *      Author: omlad & Senior Greenberg
 */

#include <assert.h>
#include <time.h>
#include <math.h>
#include "Matrix.h"
#include <stdio.h>
#include <stdlib.h>

/* load info provided into the cell: column & data*/
void makeCell (cell* cell, int column){
	cell->col = column;
	cell->nextCell = NULL;
}


/* Allocates a new linked-lists sparse matrix of size n */
Matrix* allocateMatrix(int n){
	 Matrix* matrix = (Matrix*)(calloc(1,sizeof(Matrix)));

	 matrix->n = n;
	 matrix->A = (list*) (calloc(n,sizeof(cell*)));
	 matrix->K = (int*) calloc (n, sizeof(int));
	 matrix->kmFactor = (double*) calloc(n,sizeof(double));

	 return matrix;
}

/*frees all Matrix memory*/
void freeMatrix(Matrix *matrix){
	 int j=0, n=matrix->n;
	 list current,previous;
	 for (;j<n;++j){
		 current = (matrix->A)[j];
		 previous = current;
		 while (current != NULL){
			 previous = current;
			 current = current->nextCell;
			 free(previous);
			 previous = NULL;
		 }
		 if((previous != NULL)){
			 printf("missed one cell in%s",__FUNCTION__);
			 free(previous);
		 }
	 }
	 free(matrix->K);
	 free(matrix->kmFactor);
	 free(matrix->A);
	 free(matrix);

}

void createNmatrix(Matrix* matrix){
	int j, M=matrix->M, *k=matrix->K;
	for(j=0 ; j < matrix->n ; j++){
		(matrix->kmFactor[j]) = (double)(k[j])/(M);
	}
}


/* add row i to matrix A
 * row is represented as k size (int) array of Vi neighbors
 * for each neighbor the value is 1
 */
void AddRow(Matrix *matrix, const int *newRow, int k, int i){
	int j=0 , firstElement=1;
	list current,newCell;

	((list*)matrix->A)[i] = NULL;

	for (;j<k;++j){

			newCell = (list)calloc(1,sizeof(cell));
			if (newCell == NULL){
				printf("error loading matrix, row:%d",i);
				exit(EXIT_FAILURE);
			}
			makeCell(newCell, newRow[j]);

			if(firstElement){
				current = newCell;
				((list*)matrix->A)[i] = current;
				firstElement = 0;
				assert(((list*)matrix->A)[i]->nextCell == NULL);
			}

			else{
			current->nextCell = newCell;
			assert(newCell->nextCell == NULL);
			current = current->nextCell;
			current->nextCell=NULL;
			}
		}

}

/*result = V1 -V2 +v3 */
void combineVectors(int n, double* v1, double* v2, double *v3, double* result){
	int j;
	for(j=0; j<n ; j++){
		result[j] = v1[j]-v2[j]+v3[j];
	}
}

/*multiply matrix and double vector*/

void multSparseMatrix(const Matrix *matrix, const double *vector, double *result){
	 int j, n=matrix->n, col;
	 double dotproduct;
	 list current;
	 for (j=0;j<n;++j){
		 dotproduct = (double) 0;
		 if(matrix->filter[j]){
			 current = ((matrix->A))[j];
			 while (current != NULL){
				 col = current->col;
				 if(matrix->filter[col]){
					 dotproduct += vector[col];
				 }
				 current = current->nextCell;
			 }
		 }
		 result[j] = dotproduct;

	 }
 }

void multNmatrix(const Matrix* matrix, const double *vector, double *result){
	double *v1, dotProduct, Nij;
	int i, j, *v2, n=matrix->n;
	v1 = matrix->kmFactor ; v2 = matrix->K;

	for(i=0; i<n ; i++){
		dotProduct=0;
		if(matrix->filter[i]){
			for(j=0; j<n ; j++){
				if(matrix->filter[j]){
				Nij =(double) v1[i]*v2[j];
				dotProduct += Nij*vector[j];
				}
			}
		}
		result[i] = dotProduct;
	}

}

void multUnitMatrix(const Matrix* matrix, const double *vector, double *result){
	int j, n=matrix->n;

	for(j=0; j<n ; j++){
		result[j] = (matrix->filter[j])*(matrix->norm)*vector[j];
	}

}

void MultMatrix(const Matrix *matrix, const double *vector, double *result){
	double *v1, *v2, *v3;
	int n = matrix->n;
	v1 = (double*)calloc(n,sizeof(double));
	v2 = (double*)calloc(n,sizeof(double));
	v3 = (double*)calloc(n,sizeof(double));

	/*printIntVector(matrix->filter,n,"g");*/
	multSparseMatrix(matrix, vector, v1);
	multNmatrix(matrix,vector,v2);
	multUnitMatrix(matrix, vector, v3);

	combineVectors(n, v1,v2,v3,result);

	free(v1);free(v2);free(v3);

}

/*multiply matrix and int vector*/

void multSparseMatrixInteger(const Matrix *matrix, const int *vector, double *result){
	 int j, n=matrix->n, col;
		 double dotproduct;
		 list current;
		 for (j=0;j<n;++j){
			 dotproduct = 0.0;
			 if(matrix->filter[j]){
				 current = ((matrix->A))[j];
				 while (current != NULL){
					 col = current->col;
					 if(matrix->filter[col]){
						dotproduct += (double)vector[col];
					 }
					 current = current->nextCell;
				 }
			 }
			 result[j] = dotproduct;

		 }
	 }

void multNmatrixInteger(const Matrix* matrix, const int *vector, double *result){
	double *v1, dotProduct, Nij;
	int i, j, *v2, n=matrix->n;
	v1 = matrix->kmFactor ; v2 = matrix->K;

	for(i=0; i<n ; i++){
			dotProduct=0;
			if(matrix->filter[i]){
				for(j=0; j<n ; j++){
					if(matrix->filter[j]){
					Nij =(double) v1[i]*v2[j];
					dotProduct += Nij*vector[j];
					}
				}
			}
			result[i] = dotProduct;

	}
}

void multUnitMatrixInteger(const Matrix* matrix, const int *vector, double *result){
	int j, n=matrix->n;

	for(j=0; j<n ; j++){
		result[j] = (matrix->filter[j])*(matrix->norm)*vector[j];
	}

}

void MultMatrixInteger(const Matrix *matrix, const int *vector, double *result){
	double *v1, *v2, *v3;
	int n = matrix->n;
	v1 = (double*)calloc(n,sizeof(double));
	v2 = (double*)calloc(n,sizeof(double));
	v3 = (double*)calloc(n,sizeof(double));

	multSparseMatrixInteger(matrix, vector, v1);
	multNmatrixInteger(matrix,vector,v2);
	multUnitMatrixInteger(matrix, vector, v3);

	combineVectors(n, v1,v2,v3,result);
	free(v1);free(v2);free(v3);
}


