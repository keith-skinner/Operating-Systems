//Keith Skinner
//Lab 03

#include "message.h"

float calcTemp(int previous_node_temp, int new_integrated_temp)
{
    return (previous_node_temp * 3.0F + 2.0F * new_integrated_temp) / 5.0F;
}

void shutdownNode(int id, char * name) 
{
    printf(NODE_TERMINATING, id);
    mq_unlink(name);
    mq_unlink(MONITOR_QUEUE);
}

int main(int argc, char *argv[]) 
{
	handleArgs(argc, 2);

    Name name = "";
    int id = (int)strtol(argv[1], NULL, 10);
    sprintf(name, "/" NODE_NAME_PREFIX "%d", id);

    mqd_t node_mqd = openNodeMsgQueue(name+1, name, false);
    mqd_t monitor_mqd = openMonitorMsgQueue(name+1, false);
    float temp = startingTemp(argv[2]);
    MESSG msg = { 0 };

    bool stable = false;
    do {
        msg = recieveMessage(node_mqd);
        if (msg.stable != true) {
            temp = calcTemp(temp, msg.temperature);
            sendMessage(monitor_mqd, false, id, temp);
            printf(NODE_PRINT_TEMP, argv[1], temp);
        }
    } while (!msg.stable);

    shutdownNode(id, name);
    exit(EXIT_SUCCESS);
}