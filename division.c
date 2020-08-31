
#include "division.h"
#define cn(n) ((int*)calloc(n,sizeof(int)))
#define cg ((groupList)calloc(1,sizeof(groupCell)))
#define ggc ((group*)calloc(1,sizeof(group)))

void createGroupCell(groupCell* cell , group* ingroup){
	cell->grp = ingroup;
	cell->nextGroup = NULL;
}

/* when subdivision is final storing subgroups in approproate DS */
void subDividedBySubdiviosion(group* A, group* B, int* subDivision, int n, int a, int b){
	int j, x=0, y=0;
	int *memA = (int*) calloc(a,sizeof(int));
	int *memB = (int*) calloc(b,sizeof(int));

	A->size = a; B->size = b;
	for (j=0; j<n; j++){
		if (subDivision[j] == -1){
			memA[x] =j ;
			x++;
			}
		else if (subDivision[j] == 1){
			memB[y] = j;
			y++;
		}
	}

	if(A->members == NULL){
		free (A->members);
	}
	if(B->members == NULL){
		free (B->members);
	}

	A->members = memA;
	B->members = memB;
	}

void createDivision(group* A, int n, int* g){
	int j,i=0;
	int* members = (int*)calloc(n, sizeof(int));
	for(j=0; j<n ;j++){
		if(g[j] == 1){
			members[i] = j;
			i++;
		}
	}
	members = (int*)realloc(members, i);
	A->size = i;
	A->members = members;
}

void freeDivision(division* div){
		 int j=0, n=div->size;
		 groupList current,previous;
		 for (;j<n;++j){
			 current = div->groups;
			 while (current != NULL){
				 previous = current;
				 current = current->nextGroup;
				 free(previous->grp->members);
				 /*free(previous->grp);*/
				 free(previous);
			 }
		 }
		 free(div);

	 }

int groupSize(groupCell* A){
	return (A->grp->size);
}

void setTrivialDivision(division* div, int n){
	groupList list = (groupList)calloc(1,sizeof(groupCell));
	group* gr =  (group*)(calloc(1,sizeof(group)));
	int i;
	int* trivial = (int*)(calloc(n,sizeof(int)));
	for (i=0; i<n; i++){
		trivial[i] = i;
	}
	gr->members = trivial;
	gr->size = n;
	list->nextGroup=NULL;
	list->grp=gr;
	div->groups = list;
	div->size = 1;

}

void setEmptyDivision(division* div){
	div->size = 0;
	div->groups = NULL;
}

/*removes first group from division, make g a vector representation of it & free group memory
 * new groups will be created when transformed from vector represtation back to group DS*/
void removeG(division* P, int* g){
	int j, index;
	groupList firstCell = P->groups;
	group* gr;
	gr = firstCell->grp;
	P->groups = P->groups->nextGroup;
	P->size --;
	for (j=0 ; j< gr->size ; j++){
		index= gr->members[j];
		g[index] = 1;
	}
	freeGroupCell(firstCell);

}

void add(division* div, group* gr){
	groupList oldHead = div->groups;
	groupList new = calloc(1,sizeof(groupCell));
	createGroupCell(new, gr);
	div->groups = new;
	new->nextGroup=oldHead;
	div->size ++;
}

void reOrder(division* P, division* O, group* X, group* Y){
	if (X->size ==1){
		add(O,X);
	}else{
		add(P,X);
	}

	if(Y->size ==1){
		add(O,Y);
	}else{
		add(P,Y);
	}
}

void freeGroup(group* gr){
	free(gr->members);
	free(gr);
}

void freeGroupCell(groupList cell){
	freeGroup(cell->grp);
	free (cell);
}

/*set div as a pre=set division - written just for testing filw writting*/
void madeUpDivision(division* div){
	groupList a=cg,b=cg,c=cg,d=cg,e=cg;
	group *ga=ggc, *gb=ggc, *gc=ggc, *gd=ggc, *ge=ggc;
	int *aa=cn(4), *ab=cn(5), *ac=cn(5), *ad=cn(1), *ae=cn(5);

	aa[0]=0;aa[1]=10;aa[2]=15;aa[3]=16;
	ab[0]=1;ab[1]=11;ab[2]=17;ab[3]=18;ab[4]=19;
	ac[0]=2;ac[1]=9;ac[2]=12;ac[3]=13;ac[4]=14;
	ad[0]=3;
	ae[0]=4;ae[1]=5;ae[2]=6;ae[3]=7;ae[4]=8;

	ga->size=4 ; ga->members=aa;
	gb->size=5 ; gb->members=ab;
	gc->size=5 ; gc->members=ac;
	gd->size=1 ; gd->members=ad;
	ge->size=5 ; ge->members=ae;

	a->grp = ga; a->nextGroup=b;
	b->grp = gb; b->nextGroup=c;
	c->grp = gc; c->nextGroup=d;
	d->grp = gd; d->nextGroup=e;
	e->grp = ge; e->nextGroup=NULL;

	div->groups=a;
	div->size = 5;



	}




