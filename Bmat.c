/*
 * Bmat.c
 */

#include "Bmat.h"

 /* creating spmat B from A */
 void createB(spmat *A ,int m , struct _spmat *B){
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
 void createBg(spmat *B,int *g,struct _spmat *Bg){
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

 /* calculate multiply by 2 vectors: one int , one double */
 double multVecIntDouble(int *Vint , double *Vdouble){
	 double sum = 0;
	 int i=0;

	 if(sizeof(Vint)!=sizeof(Vdouble)){
		 printf("vec's size's dont match");
		 exit(0);
	 }
	 for(;i<sizeof(Vint);++i){
		 sum=((double)Vint[i])*Vdouble[i];
	 }
	 return sum;
 }



/* calculate Q: by definition */
double calculateDeltaQ(int* s, spmat* B){
	double result, *temp=(double*)calloc(1,sizeof(B->n));
	multMatrixByIntVec(B,s,temp);
	result=multVecIntDouble(s,temp);
	return (result*0.5);


}

/* calculate eigen pair: return eigan val and store vector in vec*/
double calculateEigenPair(double* vec, spmat* BgHat, int n){
	double norm,value;
	norm = norm1(BgHat);
	shiftC(norm,BgHat);
	powerIteration(BgHat, vec, n );
	value = eigenVal(vec,BgHat,norm);
	return value;
}

/* initial array values to -1 */
void indicesStart(int* indices,int n){
	int i;
	for (i = 0; i < n; ++i) {
		indices[i]=-1;
	}
}
/* initial array unmoved to 1 if vertex on g */
int unmovedStart(int* unmoved,int n,int* s){
	int i,ng=0;
	for (i = 0; i < n; ++i) {	/*making the unmoved group represnted by array*/
			if(s[i]!=0){
				unmoved[i]=1;
				++ng;
			}
		}
	return ng;
}


void modularityMaximization(spmat* BgHat , int* s){
	double* score ,improve ;
	double Q0,maxscore,maxImprove,deltaQ=0;
	int n ,ng, i,j ,maxScoreVertex,maxImproveIndex;;
	int* unmoved,indices;
	n=BgHat->n;
	unmoved=(int*)calloc(1,sizeof(n));
	indices=(int*)calloc(1,sizeof(n));
	score=(double*)calloc(1,sizeof(n));
	improve=(double*)calloc(1,sizeof(n));
	indicesStart(indices,n);
	ng=unmovedStart(unmoved,n,s);
	while(deltaQ>=0){	/* main while according to line 31 of the alg'*/
	for (i = 0; i < ng; ++i) {	/* lines 3-20 alg4*/
			Q0 = (calculateDeltaQ(s,BgHat)*2);
			s[0]=-s[0];
			score[0]=(calculateDeltaQ(s,BgHat)*2)-Q0;
			maxscore=score[0];
			maxScoreVertex=0;
			s[0]=-s[0];

			for (j = 1; j < n; ++j) {/*for lines 6-10 on alg4 */
				if(unmoved[j]!=0){
					s[j]=-s[j];
					score[j]=(calculateDeltaQ(s,BgHat)*2)-Q0;
					s[j]=-s[j];
					if(score[j]>maxscore){
						maxScoreVertex=j;
						maxscore=score[j];
					}
				}
			}
			s[maxScoreVertex]=-s[maxScoreVertex];
			indices[i]=maxScoreVertex;
			if(i==0){
				improve[i]=score[maxScoreVertex];
				maxImprove=improve[i];
				maxImproveIndex=i;
			}else{
				improve[i]=improve[i-1]+score[maxScoreVertex];
				if(improve[i]>maxImprove){ /*maintain maximprove for next part*/
					maxImprove=improve[i];
					maxImproveIndex=i;
				}
			}
			unmoved[maxScoreVertex]=0;
		}
	for(i=ng-1;i>maxImproveIndex;--i){
		s[indices[i]]=-s[indices[i]];
	}
	if(maxImproveIndex==(ng-1)){
		deltaQ=0;
	}else{
		deltaQ=improve[maxImproveIndex];
		}
	}
	free(unmoved);
	free(indices);
	free(score);
	free(improve);
}
