#include "cpuScheduler.h"

int main()
{
    int time = 0; // simulated time

    ALGORITHM_PARAMS parameters = {NULL, "", NULL, 0, 0, NULL}; // simulation parameters

    // read the algorithm type and time quantum if necessary
    scanf("%s", parameters.algorithm);

    //check which algorithm was passed, set values accordingly
    if (strcmp(parameters.algorithm, "RR") == 0)
    {
        scanf("%d", &parameters.quantum);
        parameters.step = &rrStep;
    }
    else if (strcmp(parameters.algorithm, "FCFS") == 0)
        parameters.step = &fcfsStep;
    else if (strcmp(parameters.algorithm, "SJF") == 0)
        parameters.step = &sjfStep;
    else if (strcmp(parameters.algorithm, "SRTF") == 0)
        parameters.step = &srtfStep;
    else
    {
        printf("The job type input is not a valid input!");
        exit(EXIT_FAILURE);
    }

    // skip over the end of line marker
    scanf("\n");

    printf("\nALGORITHM: %s", parameters.algorithm);
    if (strcmp(parameters.algorithm, "RR") == 0)
        printf("%3d", parameters.quantum);
    printf("\n\n");

    createProcessTable(PROCESS_TABLE_CAPACITY); //create process table
    createReadyQueue(READY_QUEUE_CAPACITY);//create queue with size == number of processes

    readProcessTable(); //populate process table
    displayProcessTable();

    if ((parameters.newProcess = arrivingProcess(time)) != NULL)
        addProcessToReadyQueue(parameters.newProcess);

    while (processesLeftToExecute())
    {
        parameters.time = time;

        doStep(parameters.step, &parameters);

        displayTimeTick(time, parameters.cpu);

        if (parameters.cpu != NULL)
            parameters.cpu->burstTime--;

        time++;

        if ((parameters.newProcess = arrivingProcess(time)) != NULL)
            addProcessToReadyQueue(parameters.newProcess);
    }

    printAverageWaitTime();

    cleanUp();

    return EXIT_SUCCESS;
}

// step executor
void doStep(void (*func)(void *), void *param)
{
    func(param);
}

// function implementing a step of FCFS (first come first serve)
void fcfsStep(void *param)
{
    ALGORITHM_PARAMS *p = (ALGORITHM_PARAMS *) param;

    // TODO - DONE
    if (p->cpu == NULL || p->cpu->burstTime == 0) {
        if (p->cpu != NULL)
            p->cpu->offTime = p->time;
        p->cpu = fetchProcessFromReadyQueue(0);
        
        if (p->cpu != NULL)
            p->cpu->waitTime = p->time - p->cpu->offTime;
    }
}

// function implementing a step of SJF (shortest job first)
void sjfStep(void *param)
{
    ALGORITHM_PARAMS *p = (ALGORITHM_PARAMS *) param;

    //TODO - DONE
    if (p->cpu == NULL || p->cpu->burstTime == 0) {
        //take cpu off cpu
        if (p->cpu != NULL)
            p->cpu->offTime = p->time;
        p->cpu = fetchProcessFromReadyQueue(findShortestJob());
        //if there were actual jobs left, set wait-time
        if (p->cpu != NULL)
            p->cpu->waitTime = p->time - p->cpu->offTime;
    }
}

// function implementing a step of SRTF (shortest remaining time first)
void srtfStep(void *param)
{
    ALGORITHM_PARAMS *p = (ALGORITHM_PARAMS *) param;
    
    // TODO: implement
    PROCESS * shortest = getProcessFromReadyQueue(findShortestJob());
    if (p->cpu == NULL || p->cpu->burstTime == 0 || shortest->burstTime < p->cpu->burstTime ) {
        if (p->cpu != NULL) {
            p->cpu->offTime = p->time;
            if (p->cpu->burstTime != 0)
                addProcessToReadyQueue(p->cpu);
        }
        p->cpu = shortest;
        if (p->cpu != NULL)
            p->cpu->waitTime = p->time - p->cpu->offTime;
    }

}

// function implementing a step of RR (round robin)
void rrStep(void *param)
{
    ALGORITHM_PARAMS *p = (ALGORITHM_PARAMS *) param;
    static int rrCounter = 0; 

    // TODO: implement
    if (p->cpu == NULL || p->cpu->burstTime == 0 || rrCounter == 0) {
        rrCounter = p->quantum;
        if (p->cpu != NULL) {
            p->cpu->offTime = p->time;
            if (p->cpu->burstTime != 0)
                addProcessToReadyQueue(p->cpu);
        }
        p->cpu = fetchProcessFromReadyQueue(0);
        if (p->cpu != NULL)
            p->cpu->waitTime = p->time - p->cpu->offTime;
    }
    --rrCounter;
}

//fills the processTable with processes from input
int readProcessTable()
{
    PROCESS tempProcess = {"", 0, 0, 0, 0};

    char *line = NULL;
    char *currPos;
    size_t len = 0;

    int counter = 0;
    int offset = 0;

    while (getline(&line, &len, stdin) != -1)
    {
        currPos = line;
        sscanf(currPos, "%s%n", tempProcess.name, &offset);
        currPos += offset;
        sscanf(currPos, "%d%n", &tempProcess.entryTime, &offset);
        currPos += offset;
        sscanf(currPos, "%d", &tempProcess.burstTime);

        addProcessToTable(tempProcess);

        free(line);
        line = NULL;

        counter++;
    }

    free(line);

    return counter;
}

void displayTimeTick(int time, PROCESS *cpu)
{
    printf("T%d:\nCPU: ", time);
    if (cpu == NULL)
        printf("<idle>\n");
    else
        printf("%s(%d)\n", cpu->name, cpu->burstTime);

    displayQueue();
    printf("\n\n");
}
