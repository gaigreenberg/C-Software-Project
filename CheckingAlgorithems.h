/*
 * CheckingAlgorithems.h
 *
 *  Created on: 11 בספט׳ 2020
 *      Author: omlad
 */

#ifndef CHECKINGALGORITHEMS_H_
#define CHECKINGALGORITHEMS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
/*force program to stop & print where is stopped
 * forceStop(__FUNCTION__ , __LINE__ )
 * */
void forceStop( const char* func,const int line);

/*check if argc == 3*/
void checkArgc(int argc);

/*
 * check if there are edges in the graph
 * */
void checkM(int M);

/*check if fread & fwrite finished as expected*/
void REC(const char* func, int actual, int expected, int errorNum);

/*
 * check the memory was aloocated
 * checkAllocation(ptr, __FUNCTION__, __LINE__-1);
 *  */
void checkAllocation(void* pointer, const char* func, int line );

/*print douvle vector*/
void printVector(double* vector, int n,char* name);

/*print int vector*/
void printIntVector(int* vector, int n,char* name);

/*prints input file*/
void printGraph(FILE* input, int n);

/*prints output file - shpuld be completly written before*/
void checkOutPut(char *path, int n);


#endif /* CHECKINGALGORITHEMS_H_ */
