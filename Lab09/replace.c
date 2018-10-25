/**
* Name: Keith Skinner
* Lab/task: Lab 9
* Date: 10//29/2018
**/

#include "replace.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//questionable functions
bool isTail(PAGE * page) {
    return page->next == NULL;
}
bool isHead(PAGE * page) {
    return page->prev == NULL;
}


void print_pages(PAGE * pages)
{
    while (pages != NULL) {
        printf("%-3ld", pages->page);
        pages = pages->next;
    }
    printf("\n");
}


PAGE * findPage(PAGE * pages, long page)
{
    while (pages != NULL) {
        if (pages->page == page)
            return pages;
        else pages = pages->next;
    }
    return NULL;
}


long getNextPage(char * line, char ** pos)
{
    long value = strtol(line, pos, 10);
    return (*pos != line) ? value : -1;
}

void addPage(PAGE ** head, PAGE ** tail, long page, long * size)
{
    //construct new head ptr
    PAGE * newTop = malloc(sizeof(PAGE));
    newTop->page = page;
    newTop->next = *head;
    newTop->prev = NULL;
    //put newTop into empty list
    if (*head == NULL) {
        *head = newTop;
        *tail = newTop;
    }
    //put newTop into non-empty list
    else {
        (*head)->prev = newTop;
        *head = newTop;
    }
    //increase size
    ++(*size);
}

void movePage(PAGE ** head, PAGE ** tail, PAGE * referenced, long page)
{
    if (!isHead(referenced)) {
        //need to adjust tail ptr if moving element is tail.
        if (isTail(referenced))
            *tail = referenced->prev;
        //move pointers.
        referenced->prev->next = referenced->next;
        if (!isTail(referenced))
            referenced->next->prev = referenced->prev;
        (*head)->prev = referenced;
        referenced->next = *head;
        referenced->prev = NULL;
        *head = referenced;
    }
    (*head)->page = page;
}

void pageReference(PAGE ** head, PAGE ** tail, PAGE * referenced, long page)
{
    putchar('>');
    movePage(head, tail, referenced, page);
}

void pageFault(PAGE ** head, PAGE ** tail, long page, long capacity, long * size)
{
    putchar('*');
    if (*size < capacity)
        addPage(head, tail, page, size);
    else movePage(head, tail, *tail, page);
}

int main(int argc, char ** argv)
{
    freopen(argv[1], "r", stdin);
    char line[MAX_LINE_LEN] = "";
    scanf("%[^\n]\n", line);

    PAGE * head = NULL;
    PAGE * tail = NULL;
    long capacity = strtol(line, NULL, 10);
    long size = 0;

    PAGE * referenced = NULL;
    long faults = 0;

    putchar(' ');
    print_pages(head);

    scanf("%[^\n]\n", line);
    char * pos = line;
    long page = getNextPage(pos, &pos);
    while (page != -1)
    {
        referenced = findPage(head, page);
        if (referenced == NULL) {
            pageFault(&head, &tail, page, capacity, &size);
            ++faults;
        }
        else
            pageReference(&head, &tail, referenced, page);
        print_pages(head);
        page = getNextPage(pos, &pos);
    }
    printf("\nNumber of faults: %ld\n", faults);
}