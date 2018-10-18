/**
* Name: Keith Skinner
* Lab/task: Lab 7
* Date: 10/15/2018
**/

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

int ** makeTable(int size);
bool searchForCycle(int ** table, int size);
void deleteTable(int ** table, int size);

#endif