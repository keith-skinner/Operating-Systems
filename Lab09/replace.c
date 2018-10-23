/**
* Name: Keith Skinner
* Lab/task: Lab 9
* Date: 10//29/2018
**/

#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LEN 256

typedef struct page_tag{
    struct page_tag * prev;
    struct page_tag * next;
    int page;
} PAGE;

void print_pages(PAGE * pages, int capacity, int size)
{
    for (--size; size<=0; --size, --capacity)
        printf("%d ", pages->page);
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