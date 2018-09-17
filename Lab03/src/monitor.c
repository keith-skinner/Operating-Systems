//Keith Skinner
//Lab 03

#include "message.h"

typedef struct 
{
    mqd_t msqid;
    float previousTemperature;
} TEMPERATURE;

float integratedTemp(float prev, float sum) 
{
    return (2.0F * prev + sum) / 6.0F;
}

bool equal(float a, float b, float epsilon) 
{
    return fabs(a-b) < epsilon;
}

void shutdownMonitor(mqd_t monitor_mqd, TEMPERATURE nodes[], int numberOfNodes)
{
    printf(MONITOR_STABLIZED);
    mq_unlink(MONITOR_QUEUE);
    for (int i=0; i<numberOfNodes; ++i) {
        sendMessage(nodes[i].msqid, true, 0, 0.0F);
        Name name;
        sprintf(name, "/" NODE_NAME_PREFIX "%d", i);
        mq_unlink(name);
    }
    free(nodes);
}

void makeNode(char * name, char * value)
{
    pid_t pid = fork();
    if (pid == -1)
        oops("Fork Failed", EXIT_FAILURE);
    if (pid == 0)
        execlp("./node", "node", name, value, NULL);
}

//pass in offset of argv to start of node values (starts at argv+2)
void makeNodes(int argc, char * argv[]) 
{
    for (int i=0; i<argc; ++i) {
        Name name = "";
        sprintf(name, "%d", i);
        makeNode(name, argv[i]);
    }
}

TEMPERATURE * openNodeMsgQueues(char * owner, int numberOfNodes, bool unlink)
{
    TEMPERATURE * nodeData = (TEMPERATURE *)calloc(numberOfNodes, sizeof(TEMPERATURE));
    Name name = "";
    for (int i=0; i<numberOfNodes; ++i) {
        sprintf(name, "/" NODE_NAME_PREFIX "%d", i);
        nodeData[i] = (TEMPERATURE) { openNodeMsgQueue(owner, name, unlink), 0.0F };
    }
    return nodeData;
}

int main(int argc, char * argv[]) 
{
    handleArgs(argc, 3);
    int numberOfNodes = argc-2;
    mqd_t monitor_mqd = openMonitorMsgQueue("MONITOR", true);
    TEMPERATURE * nodeData = openNodeMsgQueues("MONITOR", numberOfNodes, true);
    makeNodes(numberOfNodes, argv+2);

    float temp = startingTemp(argv[1]);
    bool stabilized;
    do {
        stabilized = true;
        //send out update of integrated temp
        printf(MONITOR_PRINT_TEMP, temp);
        int sum = 0;
        for (int i=0; i<numberOfNodes; ++i) {
            sendMessage(nodeData[i].msqid, false, 0, temp);
            MESSG msg = recieveMessage(monitor_mqd);
            if (stabilized && !equal(msg.temperature, nodeData[i].previousTemperature, 0.0001))
                stabilized = false;
            sum += msg.temperature;            
            nodeData[i].previousTemperature = msg.temperature;
        }
        if (!stabilized)
            temp = integratedTemp(temp, sum);
    } while (!stabilized);

    shutdownMonitor(monitor_mqd, nodeData, numberOfNodes);
    exit(EXIT_SUCCESS);
}

