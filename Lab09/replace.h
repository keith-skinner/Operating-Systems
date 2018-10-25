/**
* Name: Keith Skinner
* Lab/task: Lab 9
* Date: 10//29/2018
**/

#ifndef LAB09_REPLACE_H
#define LAB09_REPLACE_H

#define MAX_LINE_LEN 256

typedef struct page_tag{
    struct page_tag * prev;
    struct page_tag * next;
    int page;
} PAGE;

#endif //LAB09_REPLACE_H