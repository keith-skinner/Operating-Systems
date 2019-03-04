#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define DELAY (__useconds_t)( (rand() % 5 + 1) * 1000)

void *philosopher(void *num);
void pickup(int id);
void putdown(int id);
void lockMonitor(int id);
void unlockMonitor(int id);


pthread_mutex_t monitor_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t * self = NULL;
int * state = NULL;

enum
{
    THINKING,
    HUNGRY,
    EATING
};

int numOfSeats, numOfTurns;

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage: philosophers <number of seats> <number of turns>");
        return 1;
    }

    numOfSeats = (int)strtod(argv[1], NULL);
    numOfTurns = (int)strtod(argv[2], NULL);
    srand((unsigned int)time(NULL));

    self = calloc((size_t)numOfSeats, sizeof(pthread_cond_t));
    state = calloc((size_t)numOfSeats, sizeof(int));
    pthread_t * philosopher_tid = calloc((size_t)numOfSeats, sizeof(pthread_t));

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    printf("Start a dinner for %d diners\n", numOfSeats);

    for (long i = 0; i < numOfSeats; i++)
        pthread_create(&philosopher_tid[i], NULL, philosopher, (void *) i);

    for (long i = 0; i < numOfSeats; i++)
        pthread_join(philosopher_tid[i], NULL);

    printf("Dinner is no more.\n");

    pthread_mutex_destroy(&monitor_mutex);
    for (int i=0; i<numOfSeats; ++i)
        pthread_cond_destroy(&self[i]);
    free(self);
    free(state);
    free(philosopher_tid);
    return 0;
}

void *philosopher(void *num)
{
    int id = (int)num;
    state[id] = THINKING;
    printf("Philsopher no. %d sits at the table.\n", id);

    // philosophers arrive at the table at various times
    usleep(DELAY);

    for (int i = 0; i < numOfTurns; i++)
    {
        state[id] = HUNGRY;
        printf("Philsopher no. %d gets hungry for the %d time!\n", id, i + 1);

        pickup(id);

        usleep(DELAY);
        printf("Philsopher no. %d stopped eating\n", id);

        putdown(id);

        printf("Philsopher no. %d finished turn %d\n", id, i + 1);
    }

    printf(">>>>>> Philsopher no. %d finished meal. <<<<<<\n", id);

    pthread_exit(NULL);
}

int test(int id)
{
    return state[id] != EATING;
}

int canEat(int id)
{
    return test((id + 1) % numOfSeats)
        && test((id + numOfSeats - 1) % numOfSeats);
}

void pickup(int id)
{
    //while I haven't eaten
    printf("Philsopher no. %d tries to grab chopstick %d\n", id, id);
    while (state[id] != EATING) {
        //make sure noone else can change their state
        lockMonitor(id);
        //see if I can eat
        if (canEat(id)) {
            state[id] = EATING;
            printf("Philsopher no. %d eating\n", id);
        }
        //allow someone else to change their state
        unlockMonitor(id);
    }
}


void putdown(int id)
{
    //make sure noone else can change state
    lockMonitor(id);
    printf("Philsopher no. %d has returned chopstick %d\n", id, id);
    //change my state
    state[id] = THINKING;
    //allow someone else to change their state
    unlockMonitor(id);
}

void lockMonitor(int id)
{
    //While I don't have the mutex
    while (pthread_mutex_lock(&monitor_mutex)) {
        //wait until someone gives up the mutex
        pthread_cond_wait(&self[id], &monitor_mutex);
        //and try to grab it again.
    }
}

void unlockMonitor(int id)
{
    //Give up the mutex
    pthread_mutex_unlock(&monitor_mutex);
    //tell everyone else the mutex is available
    pthread_cond_signal(&self[id]);
}
