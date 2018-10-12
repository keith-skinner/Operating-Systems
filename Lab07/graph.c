#include "graph.h"
#include <stdlib.h>

bool ** makeTable(int size)
//creates and returns a 2-D Square table of size verts
{
    bool ** table = calloc(size, sizeof(bool*));
    for (int i=0; i<size; ++i)
        table[i] = calloc(size, sizeof(bool));
    return table;
}


bool cycle(bool ** table, const int size, int vertex, bool* visited)
//Uses depth first search to see if the vertex, row vertex is involved in a cycle.
{
    visited[vertex] = true;
    for (int i=0; i<size; ++i) {
        if (table[vertex][i]) { //connected
            return (visited[i])
                   ? true
                   : cycle(table, size, i, visited);
        }
    }
    return false;
}

void disconnect(bool ** table, const int size, int vertex)
{
    for (int i = 0; i<size; ++i)
        table[vertex][i] = false;
}

bool searchForCycle(bool ** table, int size)
//Searches each vertex to see if it's involved with a deadlock
{
    bool *visited = calloc( (size_t)size, sizeof(bool) );

    for (int vertex = 0; vertex < size; ++vertex) {
        if (!visited[vertex] && cycle(table, size, vertex, visited)) {
            free(visited);
            return true;
        }
        disconnect(table, size, vertex);
    }

    free(visited);
    return false;
}