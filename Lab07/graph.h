//Keith Skinner
//Lab 07: Deadlocks

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

bool ** makeTable(int verts)
{
    bool ** table = calloc(verts, sizeof(bool*));
    for (int i=0; i<verts; ++i)
        table[i] = calloc(verts, sizeof(bool));
    return table;
}



bool cycle(bool ** table, const int size, int vert, bool* visited)
{
    visited[vert] = true;
    for (int i=0; i<size; ++i) {
        if (table[vert][i]) { //connected
            return (visited[i])
                ? true
                : cycle(table, size, i, visited);
        }
    }
    return false;
}

bool searchForCycle(bool ** table, int size) {
    bool *visited = calloc(size, sizeof(bool));

    for (int from = 0; from < size; ++from) {
        if (cycle(table, size, from, visited)) {
            free(visited);
            return true;
        }
    }

    free(visited);

    return false;
}


#endif