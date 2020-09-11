/*
 * pIter.h
 *
 *  Created on: Aug 26, 2020
 *      Author: gai
 *      need to check the includes
 */

#ifndef POWERITERATION_H_
#define POWERITERATION_H_
#include "Matrix.h"
#include "Modularity.h"
#include "CheckingAlgorithems.h"


void generateRandomVec(double* vector, int size);

/* calculate a vector's magnitude */
double computeMagnitude(double *vec , int size );

/* calculating  the epsilon codition */
int epsilonCheck(double *currVec , double *nextVec, int size);

void normalizeVector(double *vec , double mag , int size );

/* organized fuction to generate C' power iteration (self vector into vec) */
void powerIteration(Matrix* C,double* vec,int n );

#endif /* POWERITERATION_H_ */
