//Keith Skinner
//Lab 07: Deadlocks

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

bool ** makeTable(int size);
bool searchForCycle(bool ** table, int size);


#endif