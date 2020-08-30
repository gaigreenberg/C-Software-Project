#ifndef _SPMAT_H
#define _SPMAT_H


typedef struct cell{
	 int col;
	 double value;
	 struct cell* nextCell ;
}cell;
typedef cell* list;


typedef struct _spmat {
	/* Matrix size (n*n) */
	int		n;

	/* Private field for inner implementation.
	 * Should not be read or modified externally */
	list	*private;

	/* Kvec[j] is the degree of node j  */
	int* Kvec;

} spmat;


/* Allocates a new linked-lists sparse matrix of size n */
spmat* allocateMatrix(int n);

/* Multiplies matrix A by double vector v, into result (result is pre-allocated) */
void multMatrix(const struct _spmat *A, const double *v, double *result);

/* Multiplies matrix A by int vector v, into result (result is pre-allocated) */
void multMatrixByIntVec(const struct _spmat *A, const int *v, double *result);

/* Frees all resources used by A */
void freeMatrix(struct _spmat *A);

/* Adds row i the matrix. Called before any other call,
 * exactly n times in order (i = 0 to n-1) */
void AddRow (struct _spmat *A, const double *row, int i);

/*recieves a list and copy iy to A[i]*/
void insertRow(spmat* A , list inList, int i);

/* load info provided into the cell: column & data*/
void makeCell (cell* cell, int column, double data);

/* copy cell information to another cell */
void CopyCell (list origin , list copy);

/* make a deep-copied list */
list CopyList(list old);

/* make a deep-copied list */
list CopyListFiltered(list old, int* filter);

#endif
