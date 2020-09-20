/*
 * Matrix.c
 *      Author: omlad & Senior Greenberg
 *
 *
 *
 */

#include <assert.h>
#include <time.h>
#include <math.h>
#include "Matrix.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * create a cell to be added sparse matrix
 * */
void makeCell (cell* cell, int column){
	cell->col = column;
	cell->nextCell = NULL;
}

/*generate matrix = A - N + + f D
 * A   = sparse matrix
 * Nij = ki*kj/M
 * f   = row sums
 * D   = unit matrix* NORM1 - for shifted
 *   */

/* Allocates a new linked-lists sparse matrix of size n */
Matrix* allocateMatrix(int n){
	 Matrix* matrix = (Matrix*)(calloc(1,sizeof(Matrix)));

	 matrix->n = n;
	 matrix->K = 		(int*)calloc(n, sizeof(int));
	 matrix->A = 		(list*)(calloc(n,sizeof(cell*)));
	 matrix->kmFactor = (double*)calloc(n,sizeof(double));
	 matrix->f = 		(double*)calloc(n,sizeof(double));
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
	 free(matrix->f);
	 free(matrix);

}

/*
 * create kmFactor
 * that represents nxn size matrix together with K
 */
void createNmatrix(Matrix* matrix){
	int j, M=matrix->M, *k=matrix->K;
	for(j=0 ; j < matrix->n ; j++){
		(matrix->kmFactor[j]) = (double)(k[j])/(M);
	}
}

/*
 * calculate row sum of (-N)
 */
double Nrowsum(Matrix* matrix, int row){

	int j, M=matrix->M , n=matrix->n , *g=matrix->filter, *k=matrix->K;
	double sum =0;
	for (j=0;j<n;j++){
		if (g[j]){
			sum -= (double)k[row]*k[j]/M;
		}
	}
	return sum;
}

/*
 * row sum = sum of rows of (A-N)
 */
void calculateF(Matrix* matrix){
	int j, n=matrix->n;
	double sum, Nrow;
	list current;

	for (j=0; j<n ; j++){
		sum =0;
		Nrow = Nrowsum(matrix, j );
		current = matrix->A[j];
		while (current != NULL){
			if(matrix->filter[current->col]){
				sum ++;
			}
			current = current->nextCell;
		}
		matrix->f[j] = sum + Nrow;
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


/*  multiply matrix and double vector  */

/*
 * multiply sparse matrix A and double vector according to g
 * */
void multSparseMatrix(const Matrix *matrix, const double *vector, double *result){
	 int j, n=matrix->n, col;
	 double dotproduct;
	 list current;

	 for (j=0;j<n;++j){
		 dotproduct = (double) 0;
		 if(matrix->filter[j]){
			 current = matrix->A[j];
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

/*
 * multiply N matrix with double vector according to g
 * N matrix is represent by K and kmFactor
 * K*kmFActor = N -> N*vector = kmFactor*K*vector = kmFactor*(K*vector)
 * */
void multNmatrix(const Matrix* matrix, const double *vector, double *result){
	double *v1, dotProduct=0;
	int j, *v2, n=matrix->n;
	v1 = matrix->kmFactor ; v2 = matrix->K;

	for (j=0 ; j<n ; j++){ /* calculate * vector -> scalar  */
		if (matrix->filter[j]){
			dotProduct += v2[j]*vector[j];
		}
	}
	for (j=0; j<n; j++){ /* calculate kmFactor * scalar)*/
		if(matrix->filter[j]){
			result[j] = v1[j] * dotProduct;
		}
	}


}

/*
 * multiply D matrix with douvle vector according to g
 * Dv =norm1 x vec
 * */
void multUnitMatrix(const Matrix* matrix, const double *vector, double *result){
	int j, n=matrix->n;

	for(j=0; j<n ; j++){
		result[j] = (matrix->filter[j])*(matrix->norm)*vector[j];
	}
}


void multFMatrix(const Matrix* matrix, const double *vector, double *result){
	int j;
	for (j=0;j<matrix->n;j++){
		if(matrix->filter[j]){
			result[j] = matrix->f[j]*vector[j];
		}
	}
}

/* result = V1 - V2 - v3 +v4
 * v1 = Av , v2 = Nv , v3=fv, v4 = D
 *  */
void combineVectors(int n, double* v1, double* v2, double *v3, double* v4 ,double* result){
	int j;
		for(j=0; j<n ; j++){
			result[j] = v1[j]-v2[j]-v3[j] +v4[j];
		}
}

/*
 * multiply matrix with double vector according to g
 * matrix = A - N + fI + DI
 * result = Av - Nv + fv + Dv
 *
 * */
void MultMatrix(const Matrix *matrix, const double *vector, double *result){
	double *Av, *Nv, *fv, *Dv;
	int n = matrix->n;
	Av = (double*)calloc(n,sizeof(double));
	Nv = (double*)calloc(n,sizeof(double));
	fv = (double*)calloc(n,sizeof(double));
	Dv = (double*)calloc(n,sizeof(double));

	multSparseMatrix(matrix, vector, Av);
	multNmatrix(matrix,vector,Nv);
	multFMatrix(matrix,vector, fv);
	multUnitMatrix(matrix, vector, Dv);

	combineVectors(n, Av, Nv, fv, Dv, result);

	free(Av);free(Nv);free(fv);free(Dv);

}


/*  multiply matrix and int vector  */


/*
 * multiply sparse matrix A and int vector according to g
 * */
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

/*
 * multiply N matrix with int vector according to g
 * N matrix is represent by K and kmFactor
 * K*kmFActor = N -> N*vector = kmFactor*K*vector = kmFactor*(K*vector)
 * */
void multNmatrixInteger(const Matrix* matrix, const int *vector, double *result){
	double *v1, dotProduct=0;
	int j, *v2, n=matrix->n;
	v1 = matrix->kmFactor ; v2 = matrix->K;

	for (j=0 ; j<n ; j++){ /* calculate * vector -> scalar  */
		if (matrix->filter[j]){
			dotProduct += v2[j]*vector[j];
		}
	}
	for (j=0; j<n; j++){ /* calculate kmFactor * scalar)*/
		if(matrix->filter[j]){
			result[j] = v1[j] * dotProduct;
		}
	}

}

/*
 * multiply D matrix with douvle int according to g
 * Dv =norm1 x vec
 * */
void multUnitMatrixInteger(const Matrix* matrix, const int *vector, double *result){
	int j, n=matrix->n;

	for(j=0; j<n ; j++){
		result[j] = (matrix->filter[j])*(matrix->norm)*vector[j];
	}

}


void multFMatrixInteger(const Matrix* matrix, const int *vector, double *result){
	int j;
	for (j=0;j<matrix->n;j++){
		if(matrix->filter[j]){
			result[j] = matrix->f[j]*vector[j];
		}
	}
}

/*
 * multiply matrix with int vector according to g
 *  matrix = A - N + fI
 *  rsult = Av - Nv + fv
 */
void MultMatrixInteger(const Matrix *matrix, const int *vector, double *result){
	double *Av, *Nv, *fv;
	int n = matrix->n;
	Av = (double*)calloc(n,sizeof(double));
	Nv = (double*)calloc(n,sizeof(double));
	fv = (double*)calloc(n,sizeof(double));

	multSparseMatrixInteger(matrix, vector, Av);
	multNmatrixInteger(matrix,vector,Nv);
	multFMatrixInteger(matrix, vector, fv);

	combineVectorsInteger(n, Av,Nv,fv,result);
	free(Av);free(Nv);free(fv);
}

/* result = Av - Nv + fv
 *  */
void combineVectorsInteger(int n, double* Av, double* Nv, double *fv, double* result){
	int j;
	for(j=0; j<n ; j++){
		result[j] = Av[j] - Nv[j] - fv[j];
	}
}

