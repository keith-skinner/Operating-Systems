/**
* Name: Keith Skinner
* Lab/task: Lab 9
* Date: 10//29/2018
**/

#ifndef LAB09_REPLACE_H
#define LAB09_REPLACE_H

#define MAX_LINE_LEN 256

typedef struct page_tag
{
    struct page_tag * prev;
    struct page_tag * next;
    long page;
} PAGE;


PAGE * findPage(PAGE * pages, long page);
void print_pages(PAGE * pages);
long getNextPage(char * line, char ** pos);
void pageFault(PAGE ** head, PAGE ** tail, long page, long capacity, long * size);
void pageReference(PAGE ** head, PAGE ** tail, PAGE * referenced, long page);
void movePage(PAGE ** head, PAGE ** tail, PAGE * referenced, long page);
void addPage(PAGE ** head, PAGE ** tail, long page, long * size);

#endif //LAB09_REPLACE_H