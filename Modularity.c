/*
 * Bmat.c
 */

#include "Modularity.h"
#include "math.h"


/* calculating the norm-1 of matrix and store in matrix->norm*/
void calculateMatrixNorm(Matrix *matrix){
	 int i, j,nextCellIndex=-1, n = matrix->n;
	 double max, value, Nij;
	 double* colls = (double*)calloc(n,sizeof(double));
	 list current;

	for (i = 0; i < n; ++i) {
		current = matrix->A[i];
		if(current != NULL){
			nextCellIndex = current->col;
		}

		for(j=0; j<n ; j++){
			value=0;
			Nij = (matrix->kmFactor[i])*(matrix->K[j]);
			if(j == nextCellIndex){
				value += 1;
				current = current->nextCell;
				if(current != NULL){
					nextCellIndex= current->col;
				}else{
					nextCellIndex=-1;
				}
			}
			value -= Nij;
			colls[j] += fabs(value);
		}
	}
	max = colls[0];
	 for (i = 1; i < n; ++i) {
		 if(colls[i]>max){
			 max = colls[i];
		 }
	 }
	 /*printVector(colls, n, "\nmatrix columns sums:");*/
	 matrix->norm = max;
	 free(colls);

 }

/* calculate multiply of 2 vectors */
double multiplyVectors(double *v1 , double *v2, int n1, int n2){
	 double sum = 0.0;
	 int i;

	 if(n1 != n2){
		 printf("\nvec's size's dont match");
		 exit(EXIT_FAILURE);
	 }
	 for(i=0 ; i<n1 ; ++i){
		 sum += (v1[i]*v2[i]);
	 }
	 return sum;
 }

/* calculate multiply by 2 vectors: one int , one double */
double multVecIntDouble(int *Vint , double *Vdouble, int nInt , int nDouble){
	 double sum = 0;
	 int i=0;

	 if(nInt != nDouble){
		 printf("vec's size's dont match");
		 exit(EXIT_FAILURE);
	 }
	 for(;i<nInt;++i){
		 sum += ((double)Vint[i])*Vdouble[i];
	 }
	 return sum;
}

/* calculate dQ: by definition */
double calculateDeltaQ(int* s, Matrix* matrix){
	double result, *temp = (double*)calloc(matrix->n,sizeof(double));
	MultMatrixInteger(matrix,s,temp);
	result = multVecIntDouble(s,temp, matrix->n,matrix->n);
	free(temp);
	return (result*0.5);


}

/* finding eigenValue for the founded eigenVector and normalize it with norm */
double calculateEiganValue(Matrix *C, double *v, double norm){
	int n=C->n;
	double value,denominator,numerator;
	double *Cv = (double*)calloc(n,sizeof(double));

	MultMatrix(C,v,Cv);
	numerator   = multiplyVectors(v, Cv, n, n); /* num = V*C*v */
	denominator = multiplyVectors(v, v, n, n);	/* den = v*v */
	value = numerator/denominator;
	value = value - norm;

	free(Cv);
	return value;
}

/* create sub division vec from g,vec. subdiv contains {-1,0,1} */
void updateSubDivision(int* g, int* subDiv, double* vec, int n, int* a, int* b){
	int j;
	for(j=0; j<n ; j++){
			if(negative(vec[j])){
				subDiv[j] = -1*g[j];
				++(*a);

			}else if(positive(vec[j])){
				subDiv[j] = 1*g[j];
				++(*b);
			}else{
				subDiv[j]=0;
			}

	}
}

/* main calculation of modularity */

/*B should be created before, Bg,subDivision should be allocated before*/
int Alogrithem2(Matrix* matrix, int* subDiv, int* g, int n, int* a, int* b){
	double value, *vec, *nextVec, dQ;

	matrix -> filter = g;

	if( *a != 0 || *b != 0){
		printf("probelm reseting a,b");
		forceStop(__FUNCTION__, __LINE__);
	}

	vec = (double*)calloc(n, sizeof(double));
	nextVec = (double*)calloc(n,sizeof(double));

	powerIteration(matrix,vec,nextVec,n);
	value = calculateEiganValue(matrix,vec,matrix->norm);

	if (value <=0){
		return 0;
	}
	updateSubDivision(g, subDiv, vec, n, a, b);

	dQ = calculateDeltaQ(subDiv, matrix);
	if(dQ < 0){
		return 0;
	}

	free(nextVec);
	free(vec);
	return 1;
}

/*B should be created before, divisions O&P should be created before before*/
void Alogrithem3(Matrix* matrix, int n, division* O, division* P){
	groupCell *A, *B;
	int *g = (int*)calloc(n, sizeof(int)), *subDiv = (int*)calloc(n,sizeof(int));

	int a ,b, divideable , breaker=0;

	/*printf("\nA3: Entering loop:\n");*/

	while (P->DivisionSize > 0){

		a = 0 ; b = 0;
		removeG(P, g, n); /* what to doo?? */;
		divideable = Alogrithem2(matrix, subDiv,g,n,&a,&b );
		A=cg ; B=cg ;
		if(divideable){
			breaker++;
			subDividedBySubdiviosion(A,B, subDiv,n, a, b);
			reOrder(P,O,A,B);


			if(breaker == 2*n){
				printf("Algorithem 3 got stuck in LOOP");
				forceStop(__FUNCTION__, __LINE__);
			}
		}
		else{
			/*printf("\nUDA->");*/
			CreateGroupCell(A, n, g);
			add(O,A);
			freeGroupCell(B);
			B = NULL;
		}
	}
	free(subDiv);
	free(g);
	freeDivision(P);

}

/*Algorithem 4*/

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

void modularityMaximization(Matrix* BgHat , int* s){
	double *score ,*improve ;
	double Q0,maxscore,maxImprove,deltaQ=0;
	int n ,ng, i,j ,maxScoreVertex,maxImproveIndex=-1;
	int *unmoved,*indices;

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
