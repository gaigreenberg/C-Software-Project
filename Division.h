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
	#define printGroups(A,B) {printGroup(A,"A");printf("\n");printGroup(B,"B");}

	#define cn(n) ((int*)calloc(n,sizeof(int)))
	#define cg ((groupList)calloc(1,sizeof(groupCell)))
	#define ggc ((group*)calloc(1,sizeof(group)))


	typedef struct _GroupCell{
		int groupSize;		/* number of vertices in group */
		int* group;			/* vector of indices in group */
		struct _GroupCell* nextGroup;	/* next group in division */
	}groupCell;

	typedef groupCell* groupList;

	typedef struct _division{
		groupList groups;	/* a pointer to first group in divisiob */
		int DivisionSize;	/* number of groups in divisiob */
	}division;

	/*
		* create a cell conatining the group g
		 * groupCell should be allocated before
		 * will be part of linked-list
		 * */
	void CreateGroupCell(groupCell* A, int n, int* g);

	/*set div as a division with all numbers from 0 to n-1*/
	void setTrivialDivision(division* div, int n);

	/*
	 * set div as a division with no groups
	 *  */
	void setEmptyDivision(division* div);

	/*
	 * removes first group from division
	 * creates a vector representation of it & free group memory
	 * new groups will be created according to sub-division
	 * */
	void removeG(division* P, int* g, int n);

	/*
	 * add newgroup to div
	 * */
	void add(division* div, groupCell* newGroup);

	/*
	 * add A,B to O,P according to A,B sizes
	 *  */
	void reOrder(division* P, division* O, groupCell* A, groupCell* B);

	/*
	 * set groups A,B as subgroups of g
	 * according to subdivision vector
	 *  */
	void subDividedBySubdiviosion(groupCell* A, groupCell* B, int* subDivision, int n, int a, int b);

	/*
	 * free the cell
	 * */
	void freeGroupCell(groupCell* cell);

	/*
	 * free division
	 *  */
	void freeDivision(division* div);

	/* for testing */

	/*
	 * print group from cell
	 * group(groupsize) : { group indices }
	 * */
	void printGroup(groupCell *A, char* name);

	/*
	 * print division
	 * division(size)
	 * 	groups{}
	 * */
	void printDivision(division* div,char* name);



#endif /* DIVISION_H_ */
