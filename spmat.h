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

	/* Adds row i the matrix. Called before any other call,
	 * exactly n times in order (i = 0 to n-1) */
	void	(*add_row)(struct _spmat *A, const double *row, int i);

	/* Frees all resources used by A */
	void	(*free)(struct _spmat *A);

	/* Multiplies matrix A by vector v, into result (result is pre-allocated) */
	void	(*mult)(const struct _spmat *A, const double *v, double *result);

	/* Private field for inner implementation.
	 * Should not be read or modified externally */
	list	*private;

	/* Kvec[j] is the degree of node j  */
	int* Kvec;

} spmat;
/* Allocates a new linked-lists sparse matrix of size n */

spmat* spmat_allocate_list(int n);

/* load info provided into the cell: column & data*/
void makeCell (cell* cell, int column, double data);

/* copy cell information to another cell */
void CopyCell (list origin , list copy);

/* make a deep-copied list */
list CopyList(list old);

/* make a deep-copied filtered by g group list */
list CopyListFiltered(list old, int* filter);

void	mult_list( struct _spmat *A,  double *v, double *result);

#endif
