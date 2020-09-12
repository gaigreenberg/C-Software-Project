/*
 * pIter.c
 *
 *  Created on: Aug 26, 2020
 *      Author: gai
 *      this is a copy of relevent funtions of hw2
 */

#define epsilon 0.00001
#include "PowerIteration.h"
#include "Matrix.h"
#include <math.h>

/*
 * generates a randmoized (double) vector of size n
 */
void generateRandomVec(double* vector, int size){
		int i;
		for(i=0 ; i<size ; ++i){
			vector[i] =(double) rand();
		}
	}

/*
 * computes the magnitude of a (double) vector of size n
 */
double computeMagnitude(double *vec , int size ){ /* calculate a vector's magnitude */
	double mag=0.0;
	int i;
	for (i=0 ; i <size ; ++i){
		mag = mag + pow(vec[i],2.0);
	}
	if(mag == 0.0 ){
		printf("\n ->magnitude computed = 0 therefore running was terminated\n");
		exit(EXIT_FAILURE);
	}
	mag = sqrt(mag);
	return mag;
}

/*
 * checks if epsilon condition is fullfilled
 * 		returns 1 if vec is close to eigen as epsilon to zero
 */
int epsilonCheck(double *currVec , double *nextVec, int size){	/* calculating  the epsilon codition */
	int i=0 ,b=1;

	while (b == 1 && i<size ){
		if(fabs(currVec[i]-nextVec[i]) >= epsilon){
			b = 0;
		}
		++i;
	}
	return b;
}

/*
 * divides the vector by its magnitude
 */
void normalizeVector(double *vec , double mag , int size ) {
	int i;
	for (i=0 ; i <size ; ++i){
		vec[i] =(double) vec[i]/mag;
	}

}

/* organized fuction to generate C' power iteration (self vector into vec) */
void powerIteration(Matrix* matrix,double* vec,int n ){
	int iter = 0,keepRunning = 1;
	double *nextVec, magnitude;

	generateRandomVec(vec,n);
	do{
		nextVec = (double*)calloc(n,sizeof(double));
		iter++;

		MultMatrix(matrix,vec,nextVec);
		magnitude = computeMagnitude(nextVec,n);
		normalizeVector(nextVec,magnitude,n);

		if(iter == 1000){
			printf("\n break loop %d iterations\n",iter);
			keepRunning  = 1;
		}

		if(epsilonCheck(vec,nextVec,n) == 0){
			free(vec);
			vec = nextVec;
		}else{
			keepRunning = 0;
		}


	}while (keepRunning);
	/*printf("power iteration took %d iterations\n",iter);*/

	/*free(nextVec);*/
	}





