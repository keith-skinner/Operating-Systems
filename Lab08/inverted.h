/**
* Name: Keith Skinner
* Lab/task: Lab 8
* Date: 10/18/2018
**/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct proc
{
  int pid;
  int base;
  time_t timeStamp;
} PROC;

void initInverted(PROC **, int memSize, int frameSize);
long translate(PROC *, int, int, int);
