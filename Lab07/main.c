/**
* Name: Keith Skinner
* Lab/task: Lab 7
* Date: 10/15/2018
**/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "graph.h"

#define MAX_LINE_LENGTH 256

char * getOffset(char * line, char * next)
{
    int offset = 0;
    if (sscanf(line, "%[^,{} ]%n", next, &offset) == 0)
        offset = 1;
    return line += offset;
}

char ** addVertex(char * next, char ** vertices, int * numVertices)
{
    *numVertices += 1;
    vertices = realloc(vertices, (*numVertices) * sizeof(char*));
    vertices[(*numVertices)-1] = malloc(strlen(next)+1);
    strcpy(vertices[(*numVertices)-1], next);
    return vertices;
}


char ** getVertices(char * line, int * numVertices)
{
    char ** vertices = NULL;
    *numVertices = 0;

    int offset = 0;
    sscanf(line, " V = { %n", &offset);
    line += offset;

    char next[16];
    while (*line != '\0') {
        line = getOffset(line, next);
        if (next[0] != '\0')
            vertices = addVertex(next, vertices, numVertices);
        next[0] = '\0';
    }
    return vertices;
}

char * addConnection(char * next, char *connection)
{
    connection = realloc(connection, strlen(next)+1);
    strcpy(connection, next);
    return connection;
}

bool getConnection(char ** line, char * next, char *connection[2])
{
    int vertex = 0;
    while (vertex<2 && (*line)[0] != '\0') {
        *line = getOffset(*line, next);
        if (next[0] != '\0'){
            connection[vertex] = addConnection(next, connection[vertex]);
            ++vertex;
        }
        next[0] = '\0';
    }
    return vertex == 2;
}

int findVertex(char ** vertices, int size, char * vertex)
{
    int i=0;
    for (; i<size; ++i)
        if (strcmp(vertices[i], vertex) == 0)
            break;
    return i;
}

void setConnections(int ** table, char ** vertices, int size, char * line)
{
    //so I can safely realloc
    char * connection[2] = {
            calloc(0, sizeof(char)),
            calloc(0, sizeof(char))
    };

    int offset = 0;
    sscanf(line, " E = { %n", &offset);
    line += offset;

    char next[16] = "";
    while(getConnection(&line, next, connection)) {
        int from = findVertex(vertices, size, connection[0]);
        int to = findVertex(vertices, size, connection[1]);
        table[from][to] = 1;
    }

    free(connection[0]);
    free(connection[1]);
}

void output(int ** table, int size)
{
    printf("GRAPH ADJACENCY MATRIX\n\n");

    for (int row=0; row<size; ++row){
        for (int col=0; col<size; ++col)
            printf("%3d",table[row][col]);
        printf("\n");
    }
    printf("\n");

    searchForCycle(table, size)
        ? printf(">> DEADLOCKED <<\n")
        : printf(">> NO DEADLOCK <<\n");
}

void outputVertices(char ** vertices, int numVertices)
{
    printf("\nverts: %d\n", numVertices);
    for (int i=0; i<numVertices; ++i)
        printf("%-5s", vertices[i]);
    printf("\n");
}

int main()
{
    char line[MAX_LINE_LENGTH] = "";
    scanf("%[^\n]\n", line);

    int numVertices = 0;
    char ** vertices = getVertices(line, &numVertices);
    int ** table = makeTable(numVertices);
    scanf("%[^\n]", line);
    setConnections(table, vertices, numVertices, line);

    //outputVertices(vertices, numVertices);
    output(table, numVertices);
}