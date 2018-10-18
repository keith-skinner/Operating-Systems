#include "inverted.h"

long lookUp(PROC *, int, int);
long findOldest(PROC *table);

/*
 * initializes the table for the given sizes of the memory and frame
 */
void initInverted(PROC **table, int msize, int fsize)
{
    // TODO: implement
}

/*
 * translate a logical address <pid, page, offset> into a physical address
 * if there is no entry for the pid and the page number in the table, it has to be added
 * if there is no room in the table for the new entry, then the oldest entry has to be removed
 */
long translate(PROC *table, int pid, int page, int offset)
{
    // TODO: implement

    return 0;
}

/*
 * find the entry in the table for a specific pid and page
 */
long lookUp(PROC *table, int pid, int page)
{
    int i;

    // TODO: implement

    return i;
}

/*
 * find the oldest entry in the table
 */
long findOldest(PROC *table)
{
    long min;

    // TODO: implement

    return min;
}
