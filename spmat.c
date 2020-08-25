/*
 * spmat.c
 *      Author: omlad & Senior Greenberg
 */

#include <assert.h>
#include <time.h>
#include <math.h>
#include "spmat.h"
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
	list copy = calloc (1, sizeof(cell));

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
	list copy = calloc (1, sizeof(cell));

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

/* read a Row from original matrix and store all non-zero elemnts as linked-list in Matrix*/
void AddRow(struct _spmat *A, const double *row, int i){
	int j=0 , firstElement=1 , n=A->n;
	list current,newCell;

	((list*)A->private)[i] = NULL;
	current = (list)calloc(1,sizeof(cell));

	for (;j<n;++j){
		if(row[j] != 0.0){

			newCell = (list)calloc(1,sizeof(cell));
			if (newCell == NULL || current == NULL){
				freeMatrix(A);
				}
			makeCell(newCell,j,row[j]);
			assert(newCell->nextCell == NULL);
			if(firstElement){
				current = newCell;
				((list*)A->private)[i] = current;
				firstElement = 0;
				assert(((list*)A->private)[i]->nextCell == NULL);
			}
			else{
			current->nextCell = newCell;
			assert(newCell->nextCell == NULL);
			current = current->nextCell;
			current->nextCell=NULL;
			}
		}
	}
}

void insertRow(spmat* A , list inList, int i){
	list copy = CopyList(inList);
	A->private[i] = copy;
}

  /*frees all Matrix memory*/
void freeMatrix(struct _spmat *A){
	 int j=0, n=A->n;
	 list current,previous;
	 for (;j<n;++j){
		 current = ((list*)(A->private))[j];
		 while (current != NULL){
			 previous = current;
			 current = current->nextCell;
			 free(previous);
		 }
	 }
	 free(A->Kvec);
	 free(A);

 }

 /*multiply matrix A with vector v and saves the vector in result */
void multMatrix(const struct _spmat *A, const double *v, double *result){
	 int j=0, n=A->n, column;
	 double data, dotproduct;
	 list current;
	 for (;j<n;++j){
		 dotproduct = 0.0;
		 current = ((list*)(A->private))[j];
		 while (current != NULL){
			 column = current->col;
			 data = current->value;
			 dotproduct += data * v[column];
			 current = current->nextCell;
		 }
		 result[j] = dotproduct;

	 }
 }

  /* Allocates a new linked-lists sparse matrix of size n */
 spmat* allocateMatrix(int n){
	 spmat* matrix = (spmat*)(calloc(1,sizeof(spmat)));

	 matrix->n = n;
	 matrix->private = (calloc(n,sizeof(cell*)));
	 matrix->Kvec = (int*) calloc (n, sizeof(int));

	 if(&matrix->n == NULL){
		 free (matrix);
		 return NULL;
	 }
	 if(matrix->private == NULL){
		 free (matrix);
		 return NULL;
	 }
	 if(matrix->Kvec == NULL){
		 free (matrix);
		 return NULL;
	 }
	 return matrix;
 }


