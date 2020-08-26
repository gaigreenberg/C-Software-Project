/*
 * division.h
 *
 *  Created on: 25 баев„ 2020
 *      Author: omlad
 */


#ifndef DIVISION_H_
#define DIVISION_H_
#include "stddef.h"

#define cst(k) ((char*)calloc(k,sizeof(char)))
#define toDigit(c) (c-'0')


typedef struct _group{
	int size;		 /*number of vertices in group*/
	char* members;	/*vertices in group*/
}group;

typedef struct _GroupCell{
	group grp;
	struct _GroupCell* nextGroup;
}GroupCell;

typedef GroupCell* groupList;

typedef struct _division{
	groupList groups;
	int size;
}division;

void creteGroupCell(GroupCell cell , group ingroup){
	cell->grp = ingroup;
	cell->nextGroup = NULL;
}

/* when subdivision is final storing subgroups in approproate DS */
void createGroups(group* A, group* B, int* subDivision, int n, int a, int b){
	int j, x=0, y=0;
	char *strA=cst(a) , *strB=cst(b);

	A->size = a; B->size = b;
	for (j=0; j<n; j++){
		if (subDivision[j] == -1){
			strA[x] =(char)(j);
			x++;
			}
		else if (subDivision[j] == 1){
			strB[y] = (char)(j);
			y++;
		}
	}
	A->members = strA;
	B->members = strB;
	}

void freeDivision(division* div){
		 int j=0, n=div->size;
		 groupList current,previous;
		 for (;j<n;++j){
			 current = div->groups;
			 while (current != NULL){
				 previous = current;
				 current = current->nextGroup;
				 free(previous->grp.members);
				 /*free(previous->grp);*/
				 free(previous);
			 }
		 }
		 free(div);

	 }

int groupSize(GroupCell* A){
	return (A->grp.size);
}

int* groupRep(GroupCell* A){
	int j, n=(A->grp.size);
	int* members = (int*) calloc(n, sizeof(int));
	for(j=0; j<n; j++){
		members[j] = toDigit((A->grp.members)[j]);
	}
	return members;
}

	#endif /* DIVISION_H_ */
