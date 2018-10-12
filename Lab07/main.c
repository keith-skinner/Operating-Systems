#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "graph.h"

#define MAX_LINE_LENGTH BUFSIZ
#define MAX_NAME_LENGTH BUFSIZ/8

typedef struct {
    int from;
    int to;
} EDGE;

int countVerticies(char * buffer)
{
    int count = 0;
    while(strchr(buffer, ',') != NULL)
        ++count;
    return count + 1;
}

char ** getVerticies(char * buffer, const int length)
{
    int count = countVerticies(buffer);
    char

}

int main()
{
    char buffer[MAX_LINE_LENGTH] = "";
    char ** verticies = getVerticies(buffer, MAX_LINE_LENGTH);
    //char ** edges = getEdges();

    scanf("V={%s}\n");
    scanf("E={%s}\n");
        scanf("{%s, %s}")



    return 0;
}