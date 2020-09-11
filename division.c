
#include "division.h"

/*
void createGroupCell(groupCell* cell , int* ingroup, int groupSize){
	cell->group = ingroup;
	cell->nextGroup = NULL;
	cell->groupSize = groupSize;
}
*/


/*groupCell should be allocated before*/
void CreateGroupCell(groupCell* A, int n, int* g){
	int j,i=0;
	int* members = (int*)calloc(n, sizeof(int));
	for(j=0; j<n ;j++){
		if(g[j] == 1){
			members[i] = j;
			i++;
		}
	}
	members = (int*)realloc(members, i);
	A->groupSize = i;
	A->group= members;
}

void setTrivialDivision(division* div, int n){
	groupList groupCell = (groupList)calloc(1,sizeof(groupCell));
	int i;
	int* trivial = (int*)(calloc(n,sizeof(int)));
	for (i=0; i<n; i++){
		trivial[i] = i;
	}
	groupCell->groupSize = n;
	groupCell->nextGroup=NULL;
	groupCell->group = trivial;
	div->groups = groupCell;
	div->DivisionSize = 1;

}

void setEmptyDivision(division* div){
	div->DivisionSize = 0;
	div->groups = NULL;
}

/*removes first group from division, make g a vector representation of it & free group memory
 * new groups will be created when transformed from vector represtation back to group DS*/
void removeG(division* P, int* g){
	int j, index;
	groupList removedGroup = P->groups;

	P->groups = P->groups->nextGroup;
	P->DivisionSize --;
	for (j=0 ; j < (removedGroup->groupSize) ; j++){
		index = removedGroup->group[j];
		g[index] = 1;
	}
	freeGroupCell(removedGroup);

}

void add(division* div, groupCell* newGroup){
	groupList oldHead = div->groups;
	div->groups = newGroup;
	newGroup->nextGroup=oldHead;
	div->DivisionSize ++;
}

void reOrder(division* P, division* O, groupCell* A, groupCell* B){
	if (A->groupSize == 1){
		add(O,A);
	}else{
		add(P,A);
	}

	if(B->groupSize ==1){
		add(O,B);
	}else{
		add(P,B);
	}
}

/* when subdivision is final storing subgroups in approproate DS */
void subDividedBySubdiviosion(groupCell* A, groupCell* B, int* subDivision, int n, int a, int b){
		int j, x=0, y=0;
	int *groupA = (int*) calloc(a,sizeof(int));
	int *groupB = (int*) calloc(b,sizeof(int));

	A->groupSize = a; B->groupSize = b;
	for (j=0; j<n; j++){
		if (subDivision[j] == -1){
			groupA[x] =j ;
			x++;
			}
		else if (subDivision[j] == 1){
			groupB[y] = j;
			y++;
		}


	}

	if(A->group == NULL){
		free (A->group);
	}
	if(B->group == NULL){
		free (B->group);
	}

	A->group = groupA;
	B->group = groupB;

	/*printf("\n");
	printGroup(A,"A");
	printf("\n");
	printGroup(B,"B");
	printf("\n");*/

}

void freeGroupCell(groupCell* cell){
	free(cell->group);
	free(cell);
}

void freeDivision(division* div){
		 int j=0, n=div->DivisionSize;
		 groupList current,previous;
		 for (;j<n;++j){
			 current = div->groups;
			 while (current != NULL){
				 previous = current;
				 current = current->nextGroup;
				 freeGroupCell(previous);
			 }
		 }
		 free(div);

	 }

void printGroup(groupCell *A, char* name){
	int j;
	printf("group %s: {",name);
	if(A->groupSize == 0){
		printf(" empty }");
		return;
	}
	for(j=0;j<A->groupSize;j++){
		printf("%d ",A->group[j]);
	}
	printf("}");
}

/*set div as a pre=set division - written just for testing filw writting*/
void madeUpDivision(division* div){
	groupList a=cg,b=cg,c=cg,d=cg,e=cg;
	int *aa=cn(4), *ab=cn(5), *ac=cn(5), *ad=cn(1), *ae=cn(5);

	aa[0]=0;aa[1]=10;aa[2]=15;aa[3]=16;
	ab[0]=1;ab[1]=11;ab[2]=17;ab[3]=18;ab[4]=19;
	ac[0]=2;ac[1]=9;ac[2]=12;ac[3]=13;ac[4]=14;
	ad[0]=3;
	ae[0]=4;ae[1]=5;ae[2]=6;ae[3]=7;ae[4]=8;

	a->group = aa; a->groupSize=4; a->nextGroup=b;
	b->group = ab; b->groupSize=5; b->nextGroup=c;
	c->group = ac; c->groupSize=5; c->nextGroup=d;
	d->group = ad; d->groupSize=1; d->nextGroup=e;
	e->group = ae; e->groupSize=5; e->nextGroup=NULL;

	div->groups=a;
	div->DivisionSize = 5;



	}




