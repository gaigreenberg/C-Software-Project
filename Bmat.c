/*
 * Bmat.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "spmat.h"

 /* creating spmat B from A */
 void createB(struct _spmat *A ,int m , struct _spmat *B){
	 list  current;
	 double* rowB;
	 int i, j=0, column;

	 for (i = 0; i <A-> n; ++i) {
		j=0;
		rowB = (double*) calloc(A->Kvec[i], sizeof(double));
		current = ((list*)(A->private))[i];
		while(current != NULL){	/* running over A[i]*/
			 column=current->col;
			 for (; j < A->Kvec[i]; ++j) {
				 if(column==j){
					 rowB[j]=(current->value-((A->Kvec[i])*(A->Kvec[j]/m)));	/*current->value==1 allways */
					 current=current->nextCell;
				 }else{
					 rowB[j]=-((A->Kvec[i])*(A->Kvec[j]/m));
				 }
			 }
		 }
		 for (; j < A->Kvec[i]; ++j) {/* adding rest of the object if relevant*/
			 rowB[j]=((A->Kvec[i])*(A->Kvec[j]/m));
		 }
		 AddRow(B,rowB,i);
		 free(rowB);
	 }
 }

/* create B[g] by given B and group g of vertexes from V */
 void createBg(struct _spmat *B,int *g,struct _spmat *Bg){
	 int i;
	 list newlist;
	 Bg->n = B->n;

	 for (i = 0; i < B->n; ++i) {
		if(g[i] == 0){
			Bg->private[i]= NULL;}
		else {
			newlist = CopyListFiltered(B->private[i], g);
			Bg->private[i] =newlist;
		}
	}
 }

/* B^[g] */

 double RowSum(list inList){
	 double sum = 0;
	 while (inList != NULL){
		 sum += inList->value;
		 inList = inList->nextCell;
	 }
	 return sum;
 }

 void createBgHat(spmat* Bg){
	 int i , n=Bg->n;
	 double sum=0;
	 list current, next, newCell;
	 for(i=0 ; i<n ; i++){
		 sum = RowSum(Bg->private[i]);

		 if(Bg->private[i] != NULL){ /*empty row <-> sum=0 )*/
			current = Bg->private[i];
			next = current->nextCell;

			while(next != NULL && next->col <= i){
				current = next;
				next = next->nextCell;
			}
			if (current->col /= i){ /* Bg[i][i] = 0 */
				newCell=calloc(1,sizeof(cell));
				makeCell(newCell, i ,-sum);
				current->nextCell = newCell;
				newCell->nextCell = next;

			}else if(current->col == i) { /*Bg[i][i] /= 0 */
				current->value -= sum;
			}
		 }
	 }
 }

 void shiftC(double norm, spmat* C){
	 int i, n =C->n;
	 list current, next, newCell;

	 for (i=0 ; i<n ;i++){
		 if(C->private[i] == NULL){
			 newCell = calloc(1,sizeof(cell));
			 makeCell(newCell, i, norm);
			 C->private[i] = newCell;
		 }else {
			 current = C->private[i];
			 next = current->nextCell;

			 while(next != NULL && next->col <= i){
			 	current = next;
			 	next = next->nextCell;
			 }

			 if (current->col /= i){ /* Bg[i][i] = 0 */
			 				newCell=calloc(1,sizeof(cell));
			 				makeCell(newCell, i ,norm);
			 				current->nextCell = newCell;
			 				newCell->nextCell = next;

			 			}else if(current->col == i) { /*Bg[i][i] /= 0 */
			 				current->value += norm;
			 			}

		 }
	 }
 }
	 
	 /* calculating the norm-1 of spmat C */
 double norm1(struct _spmat *C){
	 int i,n = C->n;
	 double max=0;
	 double* colls = (double*)calloc(n,sizeof(double));
	 list current;

	 for (i = 0; i < n; ++i) {
		 current=((list*)(C->private))[i];
		while(current!=NULL){
			colls[current->col]+=abs(current->value);
			current=current->nextCell;
		}
	}
	 for (i = 0; i < n; ++i) {
		 if(colls[i]>max){
			 max = colls[i];
		 }
	 }
	 free(colls);
	 return max;
 }

 /* calculate multiply by 2 vectors */
 double multVec(double *v1 , double *v2){
	 double sum = 0;
	 int i=0;

	 if(sizeof(v1)!=sizeof(v2)){
		 printf("vec's size's dont match");
		 exit(0);
	 }
	 for(;i<sizeof(v1);++i){
		 sum=v1[i]*v2[i];
	 }
	 return sum;
 }

/* finding eigenValue for the founded eigenVector and normalize it with norm */
double eigenVal(double *v,struct _spmat *C,double norm){
	double value,denominator,numerator;
	double *temp=(double*)calloc(1,sizeof(v));
	multMatrix(C,v,temp);
	numerator = multVec(v,temp);
	denominator= multVec(v,v);
	value = numerator/denominator;
	value = value - norm;
	free(temp);
	return value;
}

