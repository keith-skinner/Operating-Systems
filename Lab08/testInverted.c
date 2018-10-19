#include "inverted.h"

#define MEM_SIZE 1024*1024
#define PAGE_SIZE 4096

PROC *table;

int main(int argc, char **argv) {
    printf("\n");
    initInverted(&table, MEM_SIZE, PAGE_SIZE);
    printf("First Frame:   ");
    printf("translate(1, 10, 0)       = %ld\n", translate(table, 1, 10, 0));

    printf("Second Frame:  ");
    printf("translate(1, 20, 456)     = %ld\n", translate(table, 1, 20, 456));

    printf("Second Frame:  ");
    printf("translate(1, 20, 466)     = %ld\n", translate(table, 1, 20, 466));

    printf("First Frame:   ");
    printf("translate(1, 10, 2000)    = %ld\n", translate(table, 1, 10, 2000));

    //fill pages.
    for (int i = 1; i < MEM_SIZE / PAGE_SIZE; ++i)
        translate(table, 666, i, 14);

    printf("\nFirst Frame is now oldest:\n");
    printf("First Frame:   ");
    printf("translate(table, 7, 6, 5) = %ld\n", translate(table, 7, 6, 5));

    free(table);
    return 0;
}
