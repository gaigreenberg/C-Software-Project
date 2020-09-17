
#include "Division.h"


	/*
	 * create a cell conatining the group g
	 * groupCell should be allocated before
	 * will be part of linked-list
	 * */
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

	/*
	 * set div as a division with all numbers from 0 to n-1
	 * */
	void setTrivialDivision(division* div, int n){
		groupList cell = (groupList)calloc(1,sizeof(groupCell));
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

	/*
	 * removes first group from division
	 * creates a vector representation of it & free group memory
	 * new groups will be created according to sub-division
	 * */
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

		/*printf("\n"); printGroup(removed,"removed");*/
		freeGroupCell(removed);



	}

	/*
	 * add newgroup to div
	 * */
	void add(division* div, groupCell* newGroup){

		/*printGroup(newGroup,"gr");printf("is in 'add'\n");*/

		groupCell* temp = div->groups;
		div->groups=newGroup;
		div->groups->nextGroup = temp;
		div->DivisionSize ++;

	}

	/*
	 * add A,B to O,P according to A,B sizes
	 *  */
	void reOrder(division* P, division* O, groupCell* A, groupCell* B){
		int a=A->groupSize, b=B->groupSize;



		if (a == 0){
			add(O,B);
			freeGroupCell(A);
			return;

		}else if (a == 1){
			add(O,A);

		}else{
			add(P,A);

		}
		if(b == 0){
			add(O,A);
			freeGroupCell(B);
			return;

		}else if( b==1){
			add(O,B);

		}else{
			add(P,B);
		}
	}

	/*
	 * set groups A,B as subgroups of g
	 * according to subdivision vector
	 *  */
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

	/*
	 * free the cell
	 * */
	void freeGroupCell(groupCell* cell){
		free(cell->group);
		free(cell);
	}

	/*
	 * free division
	 *  */
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
	free(div);
	}

	/* for testing */


	/*
	 * print group from cell
	 * group(groupsize) : { group indices }
	 * */
	void printGroup(groupCell *A, char* name){
	int j;
	printf("\t\t>group %s(%d): {",name,A->groupSize);
	if(A->groupSize == 0){
		printf(" empty }");
		return;
	}
	for(j=0;j<A->groupSize;j++){
		printf("%d ",A->group[j]);
	}
	printf("}\n");
}

	/*
	 * print division
	 * division(size)
	 * 	groups{}
	 * */
	void printDivision(division* div,char* name){
		int j,s = div->DivisionSize;
		groupCell* curr = div->groups;

		printf("\n\tdivision %s size: %d\n",name,s);
		for(j=0;j<s;j++){
			printGroup(curr,"");
			curr = curr->nextGroup;
		}
	}





