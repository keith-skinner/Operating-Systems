# Task description

Develop a dynamic distributed system as a network consisting of a number of nodes and a centralized monitor.
The nodes should report their "temperatures" to the monitor which, after receiving all reports and calculating an integrated "modulating temperature", sends feedback to the nodes. The feedback may be an instruction to:

* modify the node temperature using the old temperature and the received integrated temperature included in the feedback from the monitor, or
* stop processing in case of detecting no temperature changes in any of the nodes (effectively, this feedback shuts down the whole system).

The monitor process should calculate the integrated temperature following this formula:

>   ```C
>   new_integrated_temp
>       = (2 * previous_integrated_temp + sum_of_client_temps) / 6;
>   ```
    
That value is sent as part of the feedback to all nodes.

Each of the nodes then should calculate a new temperature based on the following formula:

> ```C
> new_node_temp
>     = (previous_node_temp * 3 + 2 * new_integrated_temp) / 5;
> ```
Given these two formulas, a system with four nodes (like the one shown in the examples) should stabilize in reasonable time.

# Output
Each node process should print its temperature after the temperature is recalculated in response to the feedback from the monitor:

> NODE 2 TEMPERATURE: 78

The monitor process also should print its temperature after a new integrated temperature is calculated based on the temperatures reported by the nodes:

> MONITOR TEMPERATURE: 45

After the monitor process determines that the system is stable (i.e., no node changed its temperature) and sends feedback to the node processes informing them about the stability, it should print:

> STABLE TEMPERATURE DETECTED.
> MONITOR TERMINATING...

When a node process receives feedback from the monitor that the system is stable, it should print:

> NODE 2 TERMINATING...

(assuming a node with the identifier 2) and then exit.

# Implementation

You must use **POSIX Message Queues** in the implementation. Explore the relevant man pages starting with:

> $ man mq_overview

You will find a sample application of the message queues in the archive [avg_POSIX_uni.zip](https://cilearn.csuci.edu/courses/5613/files/580597/download?wrap=1). In the application, a server node calculates an average of temperatures reported by a number of client nodes. Download the archive, unzip it to a separate directory, explore - and then compile - the code, and experiment running the application with a variety of parameters. When you understand the sample code, especially the code that implements the communication using unidirectional message queues, move on to the implementation of the task using the following guidelines.

## Monitor Process
The dynamic system should be composed of a single central monitor process, and a number of external node processes. The whole system should be started by running the monitor process that must:

* remove (using `mq_unlink()`) any message queues that could have remained after abnormal terminations (e.g., `^C`) of the past runs of the monitor, since the queues are persistent,
* create and configure (using `mq_open()`) an afferent (incoming) message queue for accepting reports from the node processes,
* for every node process, create and configure an efferent (outgoing) message queue for sending feedback to the corresponding node processes, and then
* create (using `fork()`) and start (using `execlp()`) the node processes.

After creating the queues and the node processes, the monitor should enter the "regular" operation that consists of the loop in which:

* reports from all node processes identified by their identifier are read (using `mq_receive()`) from the afferent reporting message queue,
* computes the sum of the temperatures reported by the nodes processes,
* saves the reported values for further tests on stability,
* counts the number of processes that do not change their temperatures, and
* exits when none of the node processes changed temperature in the latest cycle;otherwise, itcontinues to loop.

After the loop is exited, the monitor must send (using `mq_send()`) a termination request to all node processes by setting the flag stable in the sent message to true, and then, it must remove all queues used in the program (again, since they are persistent).

## Node Process
Each of the node processes should open (using `mq_open()`) the reporting message queue that the monitor process has already created, and then run a loop that should:

* report the node's temperature (using `mq_send()`) to the monitor using the reporting queue,
* obtain the feedback (using `mq_send()` on the feedback queue),
* terminate if the stable flag in the received message is set to true, or
* recalculate the temperature and continue otherwise.

Please note that in this solution all node processes report temperatures to the common reporting unidirectional message queue, and receive feedback through individual unidirectional feedback queues. The monitor can tell apart reports from different node processes by the node id included in the message.

Other approaches to organizing the system logic are possible, but your are required to implement your solution following these guidelines.

Command Line Parameters:
Each node process will be uniquely identified by the enumeration value assigned to it by the monitor process during the node process creation, and it is passed to the node process as the first command line argument. The initial node process temperature is given as the second command line parameter. For example:

>   ```sh
>   $ ./node 1 100
>   ```

starts a process representing node with an identifier of 1 and the initial temperature of 100.

However, please note that the node processes will be spawned by the monitor process using execlp(), so you must provide appropriate parameters to that function that correspond to the desired command line arguments in a manual invocation. You also must ensure that the executable for node process is accessible; for example:

>   ```C
>   execlp("./node", "node", nodeIdStr, argv[i + 3], NULL);
>   ```

The monitor process should accept a number of command line arguments:

the initial temperature of the monitor process,
the number of the node processes to spawn, and
the initial temperature for each of the node processes (as you see in the next snippet, they start at argv[3]).
For example, the following invocation of the monitor process:

>   ```sh
>   $ ./monitor 200 4 25 50 75 100
>   ```

creates a system with one monitor with the starting temperature 200 and four nodes with starting temperatures 25, 50, 75, and 100. The identifiers of the corresponding nodes are implied to be 0, 1, 2, and 3.

# Files
Use the following program structure:

message.h - for includes, declarations, and definitions;
monitor.c - for the code of the monitor process;
node.c - for the code of the node process.
Message Format
The following data structure must be used for all messages (i.e., from nodes to the monitor, and vice versa):

>   ```C
>   typedef struct messg {
>       bool stable;
>       int nodeId;
>       float temperature;
>   } MESSG;
>   ```

The value of the stable flag in the report message from the nodes should be ignored by the monitor, so the nodes do not need to set it. Similarly, nodeId is relevant only in the messages sent from the nodes to the monitor, so the monitor can either ignore it or set it to an arbitrarily incorrect value (e.g., -1) in the messages that it sends to the nodes.

## Data Collection
The monitor must collect the reported temperatures, so it can reason about the changes between subsequent reporting cycles. An array of elements with the following structure can not only hold such data, but also can hold the identifier of the feedback message queue associated with a particular node:

>   ```C
>   typedef struct temperature {
>       mqd_t msqid;
>       float previousTemperature;
>   } TEMPERATURE;
>   
>   TEMPERATURE nodeData[numberOfNodes];

## Mailbox Management

The names of POSIX message queues must start with '/'. Therefore, use the following naming convention:

```#define MONITOR_QUEUE "/MONITOR"```

```#define NODE_NAME_PREFIX "NODE_"```

For printing purposes it is more convenient to use node names without the preceding '/'. However, you must add '/' in front of NODE_ (in addition to appending the node number to the prefix) when you construct the name of the queue for a specific node with sprintf().

## User Management of Message Queues

You can manage message queues from a shell as well. The ipcs command lists the identifiers of all inter-process communication resources including message queues assigned to your user id by the system. The ipcrm command can be used to remove resources (including message queues) by their identifiers. For example, if the message queue identifier 163845 appears on the output from ipcs, it can be deleted with the following command:

>   ```sh
>   $ ipcrm -q 163845
>   ```
