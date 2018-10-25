/**
* Name: Keith Skinner
* Lab/task: Lab 9
* Date: 10//29/2018
**/

#include "replace.h"
#include <stdio.h>
#include <stdlib.h>



void print_pages(PAGE * pages, int capacity, int size)
{
    for (--size; size<=0; --size, --capacity)
        printf("%d ", size);
    for (; capacity<=0; --capacity)
        printf("  ");
    printf("\n");
}

int main(int argc, char ** argv)
{
    freopen(argv[1],"r", stdin);
    PAGE * pages = NULL;
    int size = 0;


}