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
/*force program to stop & print where is stopped
 * forceStop(__FUNCTION__ , __LINE__ )
 * */
void forceStop( const char* func,const int line);

/*check if argc == 3*/
void checkArgc(int argc);

/*check if fread & fwrite finished as expected*/
void REC(const char* func, int actual, int expected, int errorNum);

/*print douvle vector*/
void printVector(double* vector, int n,char* name);

/*print int vector*/
void printIntVector(int* vector, int n,char* name);

/*prints input file*/
void printGraph(FILE* input, int n);

/*prints output file - shpuld be completly written before*/
void printDivisionFile(char *outPutFile);


#endif /* CHECKINGALGORITHEMS_H_ */
