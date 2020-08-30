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
#include "pIter.h"

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

 /* calculate multiply by 2  double vectors */
 double multVec(double *v1 , double *v2);

 /* calculate multiply by 2 vectors: one int , one double */
 double multVecIntDouble(int *Vint , double *Vdouble);

/* finding eigenValue for the founded eigenVector and normalize it with norm */
double eigenVal(double *v, spmat *C,double norm);

double calculateQ(int* s, spmat* B);

double calculateEigenPair(double* vec, spmat* BgHat, int n);


#endif /* BMAT_H_ */
