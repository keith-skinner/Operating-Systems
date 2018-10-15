/**
* Name: Keith Skinner
* Lab/task: Lab 7
* Date: 10/15/2018
**/

#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int ** makeTable(int size)
//creates and returns a 2-D Square table of size verts
{
    const size_t len = (size_t)size;
    int ** table = calloc(len, sizeof(int*));
    for (int i=0; i<size; ++i)
        table[i] = calloc(len, sizeof(int));
    return table;
}


void disconnect(int ** table, const int size, int vertex)
//helper function for searchForCycle that removes any
// connections to a node that is proven to have no loop
{
    for (int i = 0; i<size; ++i) {
        table[i][vertex] = 0;
        table[vertex][i] = 0;
    }
}


bool cycle(int ** table, const int size, int vertex, bool* visited)
//helper function for searchForCycle.
//Uses depth first search to see if the vertex, row vertex is involved in a cycle.
{
    bool loop = false;
    visited[vertex] = true;
    for (int i=0; i<size; ++i) {
        if (table[vertex][i] != 0) {                        //connected
            if (!visited[i]) {                              //not yet visited!
                if (cycle(table, size, i, visited))         //is this connected node part of a cycle?
                    return true;                            //it is directly or indirectly part of a loop.
            }
            else loop = true;                               //connected and seen before! must be a loop!
        }
    }
    disconnect(table, size, vertex);                        //this vertex has proven that it and
                                                            // it's connections are not part of a loop
    return loop;
}


bool searchForCycle(int ** table, int size)
//Searches each vertex to see if it's involved with a deadlock
{
    bool *visited = calloc( (size_t)size, sizeof(bool) );
    for (int vertex = 0; vertex < size; ++vertex) {
        if (!visited[vertex] && cycle(table, size, vertex, visited)) {
            free(visited);
            return true;
        }
    }
    free(visited);
    return false;
}

void deleteTable(int ** table, int size)
{
    for (--size; size >= 0; --size)
        free(table[size]);
    free(table);
}