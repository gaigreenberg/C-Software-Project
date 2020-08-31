/*
 * Bmat.h
 *
 *  Created on: 25 баев„ 2020
 *      Author: omlad
 */

#ifndef BMAT_H_
#define BMAT_H_

/*
 * Bmat.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "spmat.h"

 /* creating spmat B from A */
 void createB(spmat *A ,int m , spmat *B);

/* create B[g] by given B and group g of vertexes from V */
 void createBg(spmat *B, int *g, spmat *Bg);

/* B^[g] */

 double RowSum(list inList);

 void createBgHat(spmat* Bg);

 void shiftC(double norm, spmat* C);

	 /* calculating the norm-1 of spmat C */
 double norm1(struct _spmat *C);

 /* calculate multiply by 2 vectors */
 double multVec(double *v1 , double *v2);

/* finding eigenValue for the founded eigenVector and normalize it with norm */
double eigenVal(double *v, spmat *C,double norm);

/*
 * to write - Senior Greenberg
 *
 * */

double calculateDeltaQ(int* s, spmat* B);

/* calculate eigen pair: return eigan val and store vector in vec*/
double calculateEigenPair(double* vec, spmat* BgHat, int n);

/* initial array values to -1 */
void indicesStart(int* indices,int n);

/* initial array unmoved to 1 if vertex on g - sub function for modularityMximization*/
int unmovedStart(int* unmoved,int n,int* s);

/* function for modularity maximization - algorithem 4 */
void modularityMaximization(spmat* BgHat , int* s);
#endif /* BMAT_H_ */
