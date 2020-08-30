/*
 * pIter.c
 *
 *  Created on: Aug 26, 2020
 *      Author: gai
 *      this is a copy of relevent funtions of hw2
 */

#define epsilon 0.00001
#include "pIter.h"
#include <math.h>

void generateRandomVec(double* vector, int size){
		int i;
		for(i=0 ; i<size ; ++i){
			vector[i] =(double) rand();
		}
	}

  /* generate B vec:
   decide by value of indicator how to generate b: read file / randomize
void generateBvector (double* vec, int indicator, int n, char* filename){
	int k;
	FILE* b = fopen(filename,"r");
	printf("indicator: %d", indicator);
	switch (indicator){
	case 4:
		generateRandomVec(vec,n);

		break;
	case 5:
		assert(fread(&k,sizeof(int),1,b) == 1);
		assert(fread(&k,sizeof(int),1,b) == 1);
		printf("read:\n");
		k = fread(vec,sizeof(double), n , b);
		assert(k==n);

		break;
	}

	printf("finshed switch-case\n");
	fclose(b);
	printf("closed b\n");


} */

double vectorMag(double *vec , int size ){ /* calculate a vector's magnitude */
	double mag=0.0;
	int i;
	for (i=0 ; i <size ; ++i){
		mag = mag + pow(vec[i],2.0);
	}
	mag = sqrt(mag);
	return mag;
}

int epsilonCheck(double *currVec , double *nextVec, int size){	/* calculating  the epsilon codition */
	int i,b;
	b=1;
	i=0;
	while (b==1 && i<size ){
		if(fabs(currVec[i]-nextVec[i])>epsilon){
			b = 0;
		}
		++i;
	}
	return b;
}

void divideVecByMag(double *vec , double mag , int size ) {
	int i;
	for (i=0 ; i <size ; ++i){
		vec[i] =(double) vec[i]/mag;
	}

}

/* organized fuction to generate C' power iteration (self vector into vec) */
void powerIteration(spmat* C,double* vec,int n ){
	int iter = 0,bool=0;
	double *nextVec,mag;

	nextVec = (double*)calloc(n,sizeof(double));
	generateRandomVec(vec,n);
	do{
		multMatrix(C,vec,nextVec);
		printf("power iteration took %d iters",iter);
		mag = vectorMag(nextVec,n);
		divideVecByMag(nextVec,mag,n);
		if(epsilonCheck(vec,nextVec,n)==0){
						vec = nextVec;
					}else{
						bool=1;
					}

				}
				while (bool==0);
	free(nextVec);
	}





