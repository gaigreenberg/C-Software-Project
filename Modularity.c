/*
 * Bmat.c
 */

#include "Modularity.h"
#include "math.h"



void printGmembers(int* g, int n){
	int j;
	printf("G = {");
	for (j=0 ; j<n ; j++){
		if (g[j]==1) printf(" %d ", j);
	}
	printf("}\n");
}

void printRowB(double* rowB, int n, int i){
	int j;
	printf("row %d = {",i);
	for (j=0 ; j<n ; j++){
		printf(" %f ", rowB[j]);
	}
	printf("}\n");
}

	 /* calculating the norm-1 of spmat C */
void calculateNorm1(Matrix *matrix){
	 int i, j,nextCellIndex=-1, n = matrix->n;
	 double max, ijValue;
	 double* colls = (double*)calloc(n,sizeof(double));
	 list current;

	for (i = 0; i < n; ++i) {
		current = matrix->A[i];
		if(current != NULL){
			nextCellIndex = current->col;
		}

		for(j=0; j<n ; j++){
			ijValue = (matrix->kmFactor[j])*(matrix->K[i]);

			if(j == nextCellIndex){
				ijValue += 1;
				current = current->nextCell;
				if(current != NULL){
					nextCellIndex= current->col;
				}else{
					nextCellIndex=-1;
				}
			}
			colls[j] += fabs(ijValue);
		}
	}
	max = colls[0];
	 for (i = 1; i < n; ++i) {
		 if(colls[i]>max){
			 max = colls[i];
		 }
	 }
	 printVector(colls, n, "matrix columns");
	 free(colls);
	 matrix->norm = max;
 }

 /* calculate multiply by 2 vectors */
double multiplyVectors(double *v1 , double *v2, int n1, int n2){
	 double sum = 0.0;
	 int i;

	 if(n1 != n2){
		 printf("\nvec's size's dont match");
		 exit(EXIT_FAILURE);
	 }
	 for(i=0;i<n1;++i){
		 sum += (v1[i]*v2[i]);
	 }
	 return sum;
 }

/* finding eigenValue for the founded eigenVector and normalize it with norm */
double eigenVal(double *v, Matrix *C,double norm){
	int n=C->n;
	double value,denominator,numerator;
	double *Av = (double*)calloc(n,sizeof(double));
	MultMatrix(C,v,Av);
	numerator   = multiplyVectors(v, Av, n, n);
	denominator = multiplyVectors(v, v, n, n);
	value = numerator/denominator;

	value = value - norm;
	free(Av);
	return value;
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

/* calculate Q: by definition */
double calculateDeltaQ(int* s, Matrix* matrix){
	double result, *temp=(double*)calloc(matrix->n,sizeof(double));
	multNmatrixInteger(matrix,s,temp);
	result = multVecIntDouble(s,temp, matrix->n,matrix->n);
	return (result*0.5);


}

/* calculate eigen pair: return eigan val and store vector in vec*/
double calculateEigenPair(Matrix* BgHat, double* vec, int n){
	double value;

	powerIteration(BgHat, vec, n );
	value = eigenVal(vec,BgHat,BgHat->norm);

	return value;
}



/* main calculation of modularity */

/*B should be created before, Bg,subDivision should be allocated before*/
int Alogrithem2(Matrix* matrix, int* subDiv, int* g, int n, int* a, int* b){
	double value, *vec, Q;
	int j;

	matrix -> filter = g;

	*a=0 ; *b=0;
	vec = (double*)calloc(n, sizeof(double));

	/*createBg(matrix,g,Bg,colSum);
	createBgHat(Bg, colSum);
	 */
	value = calculateEigenPair(matrix, vec, n);

	if (value <=0){
		return 0;
	}
	printf("in Alg2: ");
	printVector(vec,n,"eV");
	for(j=0; j<n ; j++){
		if(vec[j]<0){
			subDiv[j] = -1;

			++(*a);
		}else if(vec[j] > 0){
			subDiv[j] = 1;
			++(*b);
		}

	}
	Q = calculateDeltaQ(subDiv, matrix);
	if(Q < 0){
		return 0;
	}

	free(vec);
	return 1;
}

/*B should be created before, divisions O&P should be created before before*/
void Alogrithem3(Matrix* matrix, int n, division* O, division* P){
	groupCell *A = cg, *B=cg;
	int *g = (int*)calloc(n, sizeof(int)), *subDiv = (int*)calloc(n,sizeof(int));

	int a=0 ,b=0, divideable , breaker=0;

	/*printf("\nA3: Entering loop:\n");*/

	while (P->DivisionSize > 0){
		removeG(P, g); /* what to doo?? */;
		divideable = Alogrithem2(matrix, subDiv,g,n,&a,&b );
		printf("after Alg2: ");
		printIntVector(subDiv,n,"eigenVector");
		if(divideable){
			breaker++;
			printf("dividavle\t");
			subDividedBySubdiviosion(A,B, subDiv,n, a, b);

			printGroups(A,B);
			printf("\n");
			reOrder(P,O,A,B);
			if(breaker == 25){
				printf("Algorithem 3 got stuck in LOOP");
				forceStop(__FUNCTION__, __LINE__);
			}
		}
		else{
			printf("\n->reached un-dividavle");

			CreateGroupCell(A, n, g);
			add(O,A);
		}
	}
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
