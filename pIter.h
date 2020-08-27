/*
 * pIter.h
 *
 *  Created on: Aug 26, 2020
 *      Author: gai
 *      need to check the includes
 */

#ifndef PITER_H_
#define PITER_H_
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "spmat.h"
#include "Bmat.h"

void generateRandomVec(double* vector, int size);

/* calculate a vector's magnitude */
double vectorMag(double *vec , int size );

/* calculating  the epsilon codition */
int epsilonCheck(double *currVec , double *nextVec, int size);

void divideVecByMag(double *vec , double mag , int size );

/* organized fuction to generate C' power iteration (self vector into vec) */
void powerIteration(spmat* C,double* vec,int n );

#endif /* PITER_H_ */
