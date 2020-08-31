/*
 * division.h
 *
 *  Created on: 25 баев„ 2020
 *      Author: omlad
 */


#ifndef DIVISION_H_
#define DIVISION_H_
	#include "stddef.h"
	#include "stdlib.h"

	typedef struct _group{
		int size;		 /*number of vertices in group*/
		int* members;	/*vertices in group*/
	}group;

	typedef struct _GroupCell{
		group* grp;
		struct _GroupCell* nextGroup;
	}groupCell;

	typedef groupCell* groupList;

	typedef struct _division{
		groupList groups;
		int size;
	}division;

	void createGroupCell(groupCell* cell , group* ingroup);

	/* when subdivision is final storing subgroups in approproate DS */
	void subDividedBySubdiviosion(group* A, group* B, int* subDivision, int n, int a, int b);

	void createDivision(group* A, int n, int* g);

	void freeDivision(division* div);

	int groupSize(groupCell* A);

	void setTrivialDivision(division* div, int n);

	void setEmptyDivision(division* div);

	/*removes first group from division, creater a vector representation of it & free group memory
	 * new groups will be created when transformed from vector represtation back to group DS*/
	void removeG(division* P, int* g);

	void add(division* div, group* gr);

	void reOrder(division* P, division* O, group* X, group* Y);

	void freeGroup(group* gr);

	void freeGroupCell(groupCell* cell);

	void madeUpDivision(division* div);


#endif /* DIVISION_H_ */
