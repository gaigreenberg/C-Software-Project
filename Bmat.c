/*
 * Bmat.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "spmat.h"


 /* calculate M given matrix A - A is a spmat and vector k - degree vector  \n
  * זה ראשוני ומיותר - השארתי כדי לקבל יותר השראה על איך לכתוב את הדברים
 int calculateM(struct _spmat *A ,int* k ){
 	int i , count = 0 ;
 	int n = A->n;
 	list current;
 	for(i=0 ; i<n ; ++i ){
 		current = ((list*)(A->private))[i];
 		while(current != NULL){
 			count ++;
 			current = current -> nextCell;
 		}

 	}
 	return count;

 }*/


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
		 A->add_row(B,rowB,i);
		 free(rowB);
	 }
 }

/* create B[g] by given B and group g of vertexes from V */
 void createBg(struct _spmat *B,int *g,struct _spmat *Bg){
	 int i;
	 list newlist;

	 for (i = 0; i < B->n; ++i) {
		if(g[i]==0){	/* vertexx i dont belong to g*/
			Bg -> private[i] = NULL;
		}else {
			newlist= CopyListFiltered(B->private[i], g);
			 Bg -> private[i] = newlist;
		}
	}
 }
 /*
     *  ---------------------------------------------------------------------
     *  23.8 by gai
     *  adding functions related to part 3 of the project
     *  Leading Eigenpair
     *-----------------------------------------------------------------------
     *  */

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
 double multiVec(double *v1 , double *v2){
	 double sum = 0;
	 int i=0;

	 if(sizeof(v1)!=sizeof(v2)){
		 printf("vec's size's dont match");
		 exit;
	 }
	 for(;i<sizeof(v1);++i){
		 sum=v1[i]*v2[i];
	 }
 }




/* finding eigenValue for the founded eigenVector */
double eigenVal(double *v,struct _spmat *C){
	double value,denominator,numerator;
	double *temp=(double*)calloc(1,sizeof(v));
	mult_list(C,v,temp);
	numerator = multiVec(v,temp);
	denominator= multiVec(v,v);
	value = numerator/denominator;
	free(temp);
	return value;
}




