#ifndef _Matrix_H
#define _Matrix_H

#include "CheckingAlgorithems.h"

/* no value in cell because all values are 1*/
typedef struct cell{
	 int col;
	 struct cell* nextCell ;
}cell;

/* a list is a pointer to cell */
typedef cell* list;

typedef struct _Matrix {

	int		n; 		/* number of vertices in graph - |V| */
	int		M; 		/* number if edges in graph - |E| */
	double 	norm;	/* NORM1 of BG with trivial g */

	list*	A;		/*sparse matrix A from input */


	int* K;				/* degrees vector of nodes:    k[j] = degree(vj) */
	double* kmFactor;	/* normalized degrees:	kmFactor[j] = degree(vj)/M */

	double* f;			/* f part of matrix = rows sums */


	int* filter;		/* current subgroup of vertices as {0,1}^n */

} Matrix;



/* load info provided into the cell: column & data*/
void makeCell (cell* cell, int column);


/* Allocates a new linked-lists sparse matrix of size n */
Matrix* allocateMatrix(int n);

/* Frees all resources used by A */
void freeMatrix(Matrix *matrix);

void printMatrix(Matrix *matrix);

void createNmatrix(Matrix *matrix);

double Nrowsum(Matrix* matrix, int row);

void calculateRowSums(Matrix* matrix);

void AddRow(Matrix *matrix, const int *newRow, int k, int i);

void combineVectorsInteger(int n, double* v1, double* v2, double *v3, double* result);

void combineVectors(int n, double* v1, double* v2, double *v3, double* v4 ,double* result);

/*multiply matrix and double vector*/


/*
 * multiply sparse matrix A and double vector according to g
 * */
void multSparseMatrix(const Matrix *matrix, const double *vector, double *result);

/*
 * multiply N matrix with double vector according to g
 * N matrix is represent by K and kmFactor
 * */
void multNmatrix(const Matrix *matrix, const double *vector, double *result);

/*
 * multiply D matrix with double vector according to g
 * Dv =norm1 x vec
 * */
void multUnitMatrix(const Matrix *matrix, const double *vector, double *result);

/*
 * multiply f matrix with double vector according to g
 * f = rowSums * I
 * */
void multFMatrix(const Matrix* matrix, const double *vector, double *result);

/*
 * multiply matrix with double vector according to g
 *  matrix = A - N + D
 */
void MultMatrix(const Matrix *matrix, const double *vector, double *result);


/*multiply matrix and int vector*/


/*
 * multiply sparse matrix A and int vector according to g
 * */
void multSparseMatrixInteger(const Matrix *matrix, const int *vector, double *result);

/*
 * multiply N matrix with int vector according to g
 * N matrix is represent by K and kmFactor
 * */
void multNmatrixInteger(const Matrix *matrix, const int *vector, double *result);

/*
 * multiply D matrix with douvle int according to g
 * Dv =norm1 x vec
 * */
void multUnitMatrixInteger(const Matrix *matrix, const int *vector, double *result);

/*
 * multiply f matrix with int vector according to g
 * f = rowSums * I
 * */
void multFMatrix(const Matrix* matrix, const double *vector, double *result);

/*
 * multiply matrix with int vector according to g
 *  matrix = A - N + D
 */
void MultMatrixInteger(const Matrix *matrix, const int *vector, double *result);

#endif
