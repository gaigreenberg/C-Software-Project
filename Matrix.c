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
void makeCell (cell* cell, int column, double data){
	cell->col = column;
	cell->value = data;
	cell->nextCell = NULL;
}

/* copy cell information to another cell */
void CopyCell (list origin , list copy){
	if(origin == NULL){
		copy = NULL;
		return;
	}
	copy->col 		= origin->col;
	copy->value 	= origin->value;
	copy->nextCell 	= origin->nextCell;
}

/* make a deep-copied list */
list CopyList(list old){
	list newList=NULL , current = old, temp;
	int first = 1;
	list copy = (list)calloc (1, sizeof(cell));

	if (old == NULL){
		free(copy);
		return NULL;
	}
	while (current != NULL){
		if (first){
			CopyCell(current, newList);
			first = 0;
			current = current->nextCell;
			temp=newList;
		}
		else{
			CopyCell(current, copy);
			temp->nextCell = copy;
			temp = temp->nextCell;
			current = current->nextCell;
		}
	}

	return newList;


}

/* make a deep-copied list */
list CopyListFiltered(list old, int* filter){
	list newList=NULL , current = old, temp;
	int first = 1;
	list copy = (list)calloc (1, sizeof(cell));

	if (old == NULL){
		free(copy);
		return NULL;
	}
	while (current != NULL){
		if(filter[current->col] == 1){
			if (first){
				CopyCell(current, newList);
				first = 0;
				temp=newList;
			}
			else{
				CopyCell(current, copy);
				temp->nextCell = copy;
				temp = temp->nextCell;
			}
		}
		current = current->nextCell;

	}
	return newList;
}


/* Allocates a new linked-lists sparse matrix of size n */
Matrix* allocateMatrix(int n){
	 Matrix* matrix = (Matrix*)(calloc(1,sizeof(Matrix)));

	 matrix->n = n;
	 matrix->A = (list*) (calloc(n,sizeof(cell*)));
	 matrix->K = (int*) calloc (n, sizeof(int));
	 matrix->kmFactor = (double*) calloc(n,sizeof(double));

	 if(&matrix->n == NULL){
		 free (matrix);
		 return NULL;
	 }
	 if(matrix->A == NULL){
		 free (matrix);
		 return NULL;
	 }
	 if(matrix->K == NULL){
		 free (matrix);
		 return NULL;
	 }
	 return matrix;
}

/*frees all Matrix memory*/
void freeMatrix(Matrix *matrix){
	 int j=0, n=matrix->n;
	 list current,previous;
	 for (;j<n;++j){
		 current = ((list*)(matrix->A))[j];
		 while (current != NULL){
			 previous = current;
			 current = current->nextCell;
			 free(previous);
		 }
	 }
	 free(matrix->K);
	 free(matrix->kmFactor);
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
	current = (list)calloc(1,sizeof(cell));

	for (;j<k;++j){

			newCell = (list)calloc(1,sizeof(cell));
			if (newCell == NULL || current == NULL){
				freeMatrix(matrix);
				}
			makeCell(newCell, newRow[j], 1.0);
			assert(newCell->nextCell == NULL);
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
	 double data, dotproduct;
	 list current;
	 for (j=0;j<n;++j){
		 dotproduct = 0.0;
		 if(matrix->filter[j]){
			 current = ((matrix->A))[j];
			 while (current != NULL){
				 col = current->col;
				 if(matrix->filter[col]){
					 data = current->value;
					 dotproduct += data * vector[col];
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

	multSparseMatrix(matrix, vector, v1);
	multNmatrix(matrix,vector,v2);
	multUnitMatrix(matrix, vector, v3);

	combineVectors(n, v1,v2,v3,result);

	free(v1);free(v2);free(v3);

}


/*multiply matrix and int vector*/

void multSparseMatrixInteger(const Matrix *matrix, const int *vector, double *result){
	 int j, n=matrix->n, col;
		 double data, dotproduct;
		 list current;
		 for (j=0;j<n;++j){
			 dotproduct = 0.0;
			 if(matrix->filter[j]){
				 current = ((matrix->A))[j];
				 while (current != NULL){
					 col = current->col;
					 if(matrix->filter[col]){
						 data = current->value;
						 dotproduct += data * vector[col];
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
				result[j] = dotProduct;
			}
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


