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
	#include <stdio.h>
	#include "CheckingAlgorithems.h"


	#define allocateDivision ((division*)calloc(1,sizeof(division)))
	#define printGroups(A,B) {printf("\n");printGroup(A,"A");printf("\n");printGroup(B,"B");printf("\n");}

	#define cn(n) ((int*)calloc(n,sizeof(int)))
	#define cg ((groupList)calloc(1,sizeof(groupCell)))
	#define ggc ((group*)calloc(1,sizeof(group)))


	typedef struct _GroupCell{
		int* group;
		int groupSize;
		struct _GroupCell* nextGroup;
	}groupCell;

	typedef groupCell* groupList;

	typedef struct _division{
		groupList groups;
		int DivisionSize;
	}division;

	/*void createGroupCellOld(groupCell* cell , group* ingroup);*/

	void CreateGroupCell(groupCell* A, int n, int* g);

	void setTrivialDivision(division* div, int n);

	void setEmptyDivision(division* div);

	/*removes first group from division, creater a vector representation of it & free group memory
	 * new groups will be created when transformed from vector represtation back to group DS*/
	void removeG(division* P, int* g);

	void add(division* div, groupCell* newGroup);

	void reOrder(division* P, division* O, groupCell* A, groupCell* B);

	/* when subdivision is final storing subgroups in approproate DS */
	void subDividedBySubdiviosion(groupCell* A, groupCell* B, int* subDivision, int n, int a, int b);

	void freeGroupCell(groupCell* cell);

	void freeDivision(division* div);

	void printGroup(groupCell *A, char* name);

	/* for testing */

	void madeUpDivision(division* div);

	void printDivisionFile(char *outPutFile);

#endif /* DIVISION_H_ */
