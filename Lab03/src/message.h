//Keith Skinner
//Lab 03

#ifndef LAB_03_MESSAGE_H
#define LAB_03_MESSAGE_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <mqueue.h>


//make sure to include headers for exit and perror
#define oops(ermsg,erno) {perror(ermsg); exit(erno); }

#define E2SMALL             -E2BIG
#define MAX_MSG_SIZE        1024
#define TYPE                1

#define NAMESIZE            16
typedef char Name[NAMESIZE];

#define MONITOR_QUEUE       "/MONITOR"
#define NODE_NAME_PREFIX    "NODE_"
#define INVALID_NUMBER      "Error could not create starting temperature"

#define NODE_USAGE          "Usage: node <name> <starting temperature>"
#define NODE_PRINT_TEMP     "NODE %s TEMPERATURE: %3.3f\n"
#define NODE_TERMINATING    "NODE %d TERMINATING...\n"

#define MONITOR_USAGE       "Usage: monitor <monitor temperature> <node temperatures>\n"
#define MONITOR_PRINT_TEMP  "MONITOR TEMPERATURE: %3.3f\n"
#define MONITOR_STABLIZED   "STABLE TEMPERATURE DETECTED\n" \
                            "MONITOR TERMINATING...\n"

typedef struct messg {
    bool stable;
    int nodeId;
    float temperature;
} MESSG;


float startingTemp(char *start) 
{
    char * end = start;
    float starting_temp = strtof(start, &end);
    if (starting_temp == 0.0F && start == end) {
        printf(INVALID_NUMBER);
        exit(EINVAL);
    }
    return starting_temp;
}

void handleArgs(int argc, int n)
{
    if (argc < n) {
        printf(MONITOR_USAGE);
        exit(E2SMALL);
    }
}

struct mq_attr makeAttributes()
{
    return (struct mq_attr){
		.mq_flags = 0,
		.mq_maxmsg = 10,
		.mq_msgsize = MAX_MSG_SIZE,
		.mq_curmsgs = 0
	};
}

mqd_t openMsgQueue(const char * owner, const char * queue, const bool unlink)
{
    if (unlink)
        mq_unlink(queue);
    
    struct mq_attr attr = makeAttributes();
    mqd_t mq_des = mq_open(queue, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &attr);

    if (mq_des < 0) {
        printf("%s", owner);
        oops(": Error opening queue.", errno);
    }
    printf("%s: Message queue %s opened\n", owner, queue);

    return mq_des;
}

mqd_t openMonitorMsgQueue(char * owner, bool unlink)
{
    return openMsgQueue(owner, MONITOR_QUEUE, unlink);
}

mqd_t openNodeMsgQueue(char * owner, char * queue, bool unlink)
{
    return openMsgQueue(owner, queue, unlink);
}

void sendMessage(mqd_t mqd, bool stable, int nodeId, float temp) 
{
    MESSG msg = (MESSG) {
        .stable = stable,
        .nodeId = nodeId,
        .temperature = temp
    };
    mq_send(mqd, (const char *)&msg, sizeof(msg), TYPE);
}

MESSG recieveMessage(mqd_t mqd) 
{
    MESSG msg = {0};
    unsigned type = 0;
    mq_receive(mqd, (char *)&msg, MAX_MSG_SIZE, &type);
    return msg;
}


#endif //LAB_03_MESSAGE_H