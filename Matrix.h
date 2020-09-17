#ifndef _Matrix_H
#define _Matrix_H

#include "CheckingAlgorithems.h"


typedef struct cell{
	 int col;
	 struct cell* nextCell ;
}cell;
typedef cell* list;

typedef struct _Matrix {
	/* Matrix size (n*n) */
	int		n;
	int		M;
	double 	norm;

	list*	A;

	/* K[j] is the degree of node j
	 * kmFactor[j] = K[j]/M */
	int* K;
	double* kmFactor;

	int* filter;

} Matrix;



/* load info provided into the cell: column & data*/
void makeCell (cell* cell, int column);

/* copy cell information to another cell */
void CopyCell (list origin , list copy);

/* make a deep-copied list */
list CopyList(list old);

/* make a deep-copied list */
list CopyListFiltered(list old, int* filter);
/*new method's functions */




/* Allocates a new linked-lists sparse matrix of size n */
Matrix* allocateMatrix(int n);

/* Frees all resources used by A */
void freeMatrix(Matrix *matrix);

void printMatrix(Matrix *matrix);

void createNmatrix(Matrix *matrix);

double getNij(Matrix* matrix, int i, int j);


void AddRow(Matrix *matrix, const int *newRow, int k, int i);

void combineVectors(int n, double* v1, double* v2, double *v3, double* result);


/*multiply matrix and double vector*/

void multSparseMatrix(const Matrix *matrix, const double *vector, double *result);

void multNmatrix(const Matrix *matrix, const double *vector, double *result);

void multUnitMatrix(const Matrix *matrix, const double *vector, double *result);

void MultMatrix(const Matrix *matrix, const double *vector, double *result);

/*multiply matrix and int vector*/

void multSparseMatrixInteger(const Matrix *matrix, const int *vector, double *result);

void multNmatrixInteger(const Matrix *matrix, const int *vector, double *result);

void multUnitMatrixInteger(const Matrix *matrix, const int *vector, double *result);

void MultMatrixInteger(const Matrix *matrix, const int *vector, double *result);

#endif
