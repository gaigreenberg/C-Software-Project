/*
 * Bmat.h
 *
 *  Created on: 25 áàåâ× 2020
 *      Author: omlad
 */

#ifndef MODULARITY_H_
#define MODULARITY_H_

/*
 * Bmat.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "Matrix.h"
#include "PowerIteration.h"




	 /* calculating the norm-1 of Matrix C */
 void calculateNorm1(struct _Matrix *C);

 /* calculate multiply by 2 vectors */
 double multiplyVectors(double *v1 , double *v2, int n1, int n2);

/* calculate multiply by 2 vectors: one int , one double */
 double multVecIntDouble(int *Vint , double *Vdouble, int nInt, int nDouble);

/* finding eigenValue for the founded eigenVector and normalize it with norm */
double eigenVal(double *v, Matrix *C,double norm);


double calculateDeltaQ(int* s, Matrix* B);

/* calculate eigen pair: return eigan val and store vector in vec*/
double calculateEigenPair(Matrix* BgHat, double* vec, int n);

/* initial array values to -1 */
void indicesStart(int* indices,int n);

/* initial array unmoved to 1 if vertex on g - sub function for modularityMximization*/
int unmovedStart(int* unmoved,int n,int* s);

/* function for modularity maximization - algorithem 4 */
void modularityMaximization(Matrix* BgHat , int* s);

void forceStop(const char* func, const int line);

#endif /* MODULARITY_H_ */
