/**
* Name: Keith Skinner
* Lab/task: Lab 8
* Date: 10/18/2018
**/

#include <stdbool.h>
#include "outfiles/inverted.h"

long lookUp(PROC *, int, int);
long findOldest(PROC *table);

//Did I have to add these?
int MSIZE = 0;
int FSIZE = 0;
int NPAGES = 0;

/*
 * initializes the table for the given sizes of the memory and frame
 */
void initInverted(PROC **table, int memSize, int frameSize)
{
    // TODO: implement
    MSIZE = memSize;
    FSIZE = frameSize;
    NPAGES = MSIZE/FSIZE;
    *table = calloc((size_t)NPAGES, sizeof(PROC));
    printf( "Paged Memory:      %d (%dKB)\n"
            "Frame Size:        %d\n"
            "Number Of Pages:   %d\n"
            "\n",
            MSIZE, MSIZE/1024, FSIZE, NPAGES);
    //for (size_t i=0; i<NPAGES; ++i) { /*random setup here? or no?*/ }
}


/*
 * translate a logical address <pid, page, offset> into a physical address
 * if there is no entry for the pid and the page number in the table, it has to be added
 * if there is no room in the table for the new entry, then the oldest entry has to be removed
 */
long translate(PROC *table, int pid, int page, int offset)
{
    // TODO: implement
    long index = lookUp(table, pid, page);
    if (index == -1) {
        //page fault.
        index = findOldest(table);
        table[index].pid = pid;
        table[index].base = page;
    }
    table[index].timeStamp = time(NULL);
    return index * FSIZE + offset;
}



/*
 * find the entry in the table for a specific pid and page
 */
long lookUp(PROC *table, int pid, int page)
{
    // TODO: implement
    for (int i = 0; i < NPAGES; ++i)
        if (table[i].pid == pid && table[i].base == page)
            return i;
    return -1;
}


/*
 * find the oldest entry in the table
 */
long findOldest(PROC *table)
{
    // TODO: implement
    long oldest = 0;
    for (long i = 1; i < NPAGES; ++i)
        if (table[i].timeStamp < table[oldest].timeStamp)
            oldest = i;
    return oldest;
}
