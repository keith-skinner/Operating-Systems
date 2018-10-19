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
