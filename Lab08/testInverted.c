#include "inverted.h"

#define MEM_SIZE 1024*1024
#define PAGE_SIZE 4096

PROC *table;

int main(int argc, char **argv)
{
   initInverted(&table, MEM_SIZE, PAGE_SIZE);
   printf("translate(1, 10, 0) = %ld\n", translate(table, 1, 10, 0));
   printf("translate(1, 20, 456) = %ld\n", translate(table, 1, 20, 456));
   return 0;
}
