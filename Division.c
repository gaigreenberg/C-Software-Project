
#include "Division.h"


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
		members = (int*)realloc(members, i*sizeof(int));
		A->groupSize = i;
		A->group= members;
		A->nextGroup = NULL;
	}

	void setTrivialDivision(division* div, int n){
		groupList cell = (groupList)calloc(1,sizeof(cell));
		int i;
		int* trivial = (int*)(calloc(n,sizeof(int)));
		for (i=0; i<n; i++){
			trivial[i] = i;
		}
		cell->groupSize = n;
		cell->nextGroup=NULL;
		cell->group = trivial;
		div->groups = cell;
		div->DivisionSize = 1;

	}

	void setEmptyDivision(division* div){
		div->DivisionSize = 0;
		div->groups = NULL;
	}

	/*removes first group from division, make g a vector representation of it & free group memory
	 * new groups will be created when transformed from vector represtation back to group DS*/
	void removeG(division* P, int* g, int n){
		groupList removed = P->groups;
		int* group = removed->group;
		int j, k=removed->groupSize, i = 0;

		P->groups = P->groups->nextGroup;
		removed->nextGroup = NULL;
		P->DivisionSize --;

		for(j=0 ; j<n ; j++){
			if((i<k) && (j == group[i])) {
				g[j] = 1;
				i++;
			}else{
				g[j]=0;
			}
		}
		printf("\n"); printGroup(removed,"removed");
		freeGroupCell(removed);

	}

	void add(division* div, groupCell* newGroup){

		printGroup(newGroup,"gr");printf("is in 'add'\n");

		groupCell* temp = div->groups;
		div->groups=newGroup;
		div->groups->nextGroup = temp;
		div->DivisionSize ++;

	}

	void reOrder(division* P, division* O, groupCell* A, groupCell* B){
		int a=A->groupSize, b=B->groupSize;



		if (a == 0){
			printf("\na=0\t");
			add(O,B);
			free(A);
			return;

		}else if (a == 1){
			printf("\na=1\t");
			add(O,A);

		}else{
			printf("\na>1\t");
			add(P,A);

		}
		if(b == 0){
			printf("\nb=0\t");
			add(O,A);
			free(B);
			return;

		}else if( b==1){
			printf("\nb=1\t");
			add(O,B);

		}else{
			printf("\nb>1\t");
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
				 previous = current;
				 while (current != NULL){
					 previous = current;
					 current = current->nextGroup;
					 freeGroupCell(previous);
				 }
				 if ( previous != NULL){
					 freeGroupCell(previous);
				 }
			 }

		 }

	void printGroup(groupCell *A, char* name){
	int j;
	printf("group %s(%d): {",name,A->groupSize);
	if(A->groupSize == 0){
		printf(" empty }");
		return;
	}
	for(j=0;j<A->groupSize;j++){
		printf("%d ",A->group[j]);
	}
	printf("} ");
}

	void printDivision(division* div,char* name){
		int j,s = div->DivisionSize;
		groupCell* curr = div->groups;

		printf("\ndivision %s size: %d\n",name,s);
		for(j=0;j<s;j++){
			printIntVector(curr->group,curr->groupSize, "\t");
			printf("\n");
			curr = curr->nextGroup;
		}
	}
	/* for testing */
	void DivPrint(division* div, char* name){
		groupCell* cell = div->groups;
		int j;
		printf("%s of size %d:\n",name, div->DivisionSize);
		for(j=0; j< div->DivisionSize; j++){
			printIntVector(cell->group,cell->groupSize, "");
			cell = cell->nextGroup;
		}
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


