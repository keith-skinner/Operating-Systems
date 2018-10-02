#include "processQueue.h"

//A dynamically allocating array of default size PROCESS_TABLE_CAPACITY

PROCESS *processTable = NULL; // an array of all processes
int processTableSize = 0;
int processTableCapacity = 0;

PROCESS **readyQueue = NULL; // an array of pointers to the processes in the processTable
int readyQueueSize = 0;
int readyQueueCapacity = 0;

// constructor of the process table
void createProcessTable(int capacity)
{
    // TODO - DONE - Allocate space for Process Table or Resize table
    if (capacity > processTableCapacity) {
        if (processTable == NULL)
            processTable = calloc(capacity, sizeof(PROCESS));
        else realloc(processTable, capacity);
        processTableCapacity = capacity;
    }
}

// constructor of the ready queue
void createReadyQueue(int capacity)
{
    // TODO - DONE - Allocate space for Ready Queue or Resize table
    if (capacity > readyQueueCapacity) {
        if (readyQueue == NULL)
            readyQueue = (PROCESS **) malloc(capacity * sizeof(PROCESS *));
        else realloc(readyQueue, capacity);
        readyQueueCapacity = capacity;
    }
}

// adds a process and expands the table if necessary
void addProcessToTable(PROCESS process)
{
    // TODO - DONE - add process to table
    if (processTableSize == processTableCapacity)
        createProcessTable(processTableCapacity * 3 / 2);
    memcpy(&processTable[processTableSize++], &process, sizeof(PROCESS));
}

void displayProcessTable()
{
    printf("PROCESSES:\nName\tEntry\tBurst\n");
    // TODO - DONE - print all processes in table
    for (int i = 0; i<processTableSize; ++i)
        printf("  %s\t  %d\t  %d\n", 
                processTable[i].name, 
                processTable[i].entryTime, 
                processTable[i].burstTime);
    printf("\n");
}

// finds the shortest job in the ready queue and returns its location in the queue
//      returns -1 if readyQueue is empty.
int findShortestJob()
{
    if (readyQueueSize == 0)
        return -1;
    
    
    // TODO - DONE - Find index to the shortest job in the queue
    int shortest = 0;
    for (int i=1; i<readyQueueSize; ++i)
        if (readyQueue[i]->burstTime < readyQueue[shortest]->burstTime)
            shortest = i;
    return shortest;
}

// adds any processes that arrive at the current time to the readyQueue
//      If no process at this time, return NULL
//      Implies only one process at a time.
PROCESS * arrivingProcess(int time)
{
    // TODO - DONE - return p
    for (int i = 0; i<processTableSize; ++i)
        if (processTable[i].entryTime == time)
            return &processTable[i];
    return NULL;
}

// determines if any processes in the process queue have more to execute
bool processesLeftToExecute()
{
    // TODO - DONE - Find if there's any processes left to execute
    int a = 0;
    for (int i=0; i<processTableSize; ++i) {
        if (processTable[i].burstTime > 0)
            ++a;
    }
    return a != 0;
}


// adds a pointer and expands the ready queue if necessary
void addProcessToReadyQueue(PROCESS *pointer)
{
    // TODO - DONE - add process to Queue
    if (readyQueueSize == readyQueueCapacity)
        createReadyQueue(readyQueueCapacity * 3 / 2);
    readyQueue[readyQueueSize++] = pointer;
}

// exposes a specific process in the ready queue (it stays in the queue)
PROCESS *getProcessFromReadyQueue(int index)
{
	return (0 < index && index < readyQueueSize) ?
		readyQueue[index] : NULL;
}

// gets a specific process from the ready queue (it gets removed from the queue)
PROCESS *fetchProcessFromReadyQueue(int index)
{
    PROCESS *proc = readyQueue[index];
    removeProcessFromReadyQueue(index);
    return proc;

    //return removeProcessFromReadyQueue(index);
}

// removes the process at index from the ready queue and returns a pointer to it
PROCESS *removeProcessFromReadyQueue(int index)
{

    PROCESS *removed = NULL;
    // TODO - DONE
    if (0 <= index && index < readyQueueSize) {
        removed = readyQueue[index];
        for (int i = index; i<readyQueueSize-1; ++i)
            readyQueue[i] = readyQueue[i+1];
        --readyQueueSize;
    }
    return removed;
}

// get size of the ready queue
int getReadyQueueSize()
{
    return readyQueueSize;
}

// displays the contents of the ready queue
void displayQueue()
{
    // TODO - DONE
    printf("QUEUE:");

    //queue is empty
    if (!processesLeftToExecute()) {
        printf(" <empty>\n");
        return;
    }

    //queue is not empty
    for (int i=0; i<readyQueueSize; ++i)
        printf(" %s(%d)", readyQueue[i]->name, readyQueue[i]->burstTime);
    printf("\n");
}

//calculates the average wait time using information in the proc_queue
//and prints the value.
void printAverageWaitTime()
{
    // TODO - DONE
    float sum = 0.0;
    for (int i=0; i<processTableSize; ++i)
        sum += (float)processTable[i].waitTime;
    printf("Average Wait Time: %.2f\n", sum/processTableSize);
}

// clean-up the table and the queue
void cleanUp()
{
    free(processTable);
    free(readyQueue);
}




