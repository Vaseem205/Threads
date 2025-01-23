/*

The pthread_barrier functionality is a synchronization mechanism used in multithreading to coordinate the execution of multiple threads
at a specific point, known as a barrier. This allows threads to wait for each other to reach a certain stage in their execution before proceeding.

In simple words, there'll be a barrier which is programmed to stop the executio till it has required number of threads waiting for it.

Ex: If a barrier is programmed to stop execution till it gets 3 threads, then the executiion will be stopped for the 1st and 2nd thread, only when 3rd thread arrivess,
do you see firther execution.

*/

// Simple Barrier Program

#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>


/*
#define THREADS_NUM 10


pthread_barrier_t barrier;


void* threadFunc(void* args){
    while(1){
        int index = *(int *)args;
        printf("Waiting for threads\n");
        sleep(1);
        pthread_barrier_wait(&barrier);
        printf("Executing, got thread[%d]\n", index);
        sleep(1);
    }
    free(args);
}

int main(){


    pthread_t th[THREADS_NUM];
    pthread_barrier_init(&barrier, NULL, 5);

    for(int i = 0; i < THREADS_NUM; i++){
        int* a = malloc(sizeof(int));
        *a = i;
        if(pthread_create(&th[i], NULL, &threadFunc, (void*)a) != 0){
            perror("Failed to create thread");
        }
    }

    for(int i = 0; i < THREADS_NUM; i++){
        int* a = malloc(sizeof(int));
        *a = i;
        if(pthread_join(th[i], NULL) != 0){
            perror("Failed to join thread");
        }
    }
    
    pthread_barrier_destroy(&barrier);
    return 0;
}

*/

/*
Waiting for threads
Waiting for threads
Waiting for threads
Waiting for threads
Waiting for threads
Waiting for threads
Waiting for threads
Waiting for threads
Waiting for threads
Waiting for threads
Executing, got thread[3]
Executing, got thread[2]
Executing, got thread[1]
Executing, got thread[0]
Executing, got thread[9]
Executing, got thread[6]
Executing, got thread[7]
Executing, got thread[4]
Executing, got thread[5]
Executing, got thread[8]
...
...
...
*/


//Practical Application

/*

There will be 8 threads.
Each thread rolls a dice
Whatever value that particular thread generated will get svaed in an array
Then, the main thread checks the values in array and declare winner.
Each thread prints weather they are winner or not.

*/

#define THREAD_NUM 8  // dice will be rolled 8 times (by diff threads)

int dice_values[8];      // Each time the dice get roled, it saves the value in this array

/*
int status[8] = {0};    
It'll get utlized by main thread to determine which thread has won or lost. 
It'll bascically parse through the diceValue array and checks what is the max value in the thread.
Then, whichever thread has that max value will be considered as won.
Bascically if you encounter the max value, set the status value 1 in that particular index 
*/
int status[8] = {0};   

pthread_barrier_t barrierRolledDice;
pthread_barrier_t barrierCalculated;

void* roll(void* args) {
    int index = *(int*)args;
    while (1) {
        dice_values[index] = rand() % 32 + 1;
        pthread_barrier_wait(&barrierRolledDice);
        pthread_barrier_wait(&barrierCalculated);
        if (status[index] == 1) {
            printf("(%d rolled %d)\t I won\n", index, dice_values[index]);
        } else {
            printf("(%d rolled %d)\t I lost\n", index, dice_values[index]);
        }
    }
    free(args);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    int i;
    pthread_barrier_init(&barrierRolledDice, NULL, THREAD_NUM + 1);
    pthread_barrier_init(&barrierCalculated, NULL, THREAD_NUM + 1);
    for (i = 0; i < THREAD_NUM; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &roll, (void*) a) != 0) {
            perror("Failed to create thread");
        }
    }

    while (1) {
        pthread_barrier_wait(&barrierRolledDice);   // Waits for all threads to roll their dice.
        // Calculate winner
        int max = 0;
        for (i = 0; i < THREAD_NUM; i++) {
            if (dice_values[i] > max) {
                max = dice_values[i];
            }
        }

        for (i = 0; i < THREAD_NUM; i++) {
            if (dice_values[i] == max) {
                status[i] = 1;
            } else {
                status[i] = 0;
            }
        }
        sleep(1);
        printf("==== New round starting ====\n");
        pthread_barrier_wait(&barrierCalculated);   // All 8 worker threads to finish processing their win/loss result (printing I won or I lost)
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    pthread_barrier_destroy(&barrierRolledDice);
    pthread_barrier_destroy(&barrierCalculated);
    return 0;
}

/*
==== New round starting ====
(3 rolled 27)	 I won
(1 rolled 8)	 I lost
(4 rolled 6)	 I lost
(2 rolled 25)	 I lost
(5 rolled 19)	 I lost
(0 rolled 15)	 I lost
(6 rolled 18)	 I lost
(7 rolled 10)	 I lost
==== New round starting ====
(1 rolled 25)	 I won
(5 rolled 22)	 I lost
(6 rolled 7)	 I lost
(4 rolled 18)	 I lost
(7 rolled 24)	 I lost
(3 rolled 9)	 I lost
(0 rolled 19)	 I lost
(2 rolled 1)	 I lost
...
...
...

*/


/*

Explnation:

1. barrierRolledDice
    Think of this barrier as:

    "Nobody calculates the winner until all workers have rolled their dice."

    All 8 workers roll their dice (dice_values[i]).

    The supervisor (main thread) is also part of the synchronization because it must wait for all workers to finish rolling before calculating the winner.

    Participants involved:
        8 workers (dice rollers)
        1 supervisor (main thread)
        So, we need 9 participants (THREAD_NUM + 1) for this barrier.

2. Think of this barrier as:

    "Nobody starts the next round until the supervisor finishes calculating the winner AND all workers acknowledge their win/loss status by printing it in roll()."

    After the supervisor calculates the winner:

        The workers (threads) check their status and print their win/loss message.
        The workers must wait for the supervisor to start the next round.
        The supervisor also waits for the workers to process their results before starting the next round.

    Participants involved:
        8 workers (to acknowledge their win/loss)
        1 supervisor (to ensure everyone is ready for the next round)
        Again, we need 9 participants (THREAD_NUM + 1) for this barrier.

-----------------------------------------------------------------------------
Step-by-Step Execution:
Main thread starts worker threads.
    Main thread creates the worker threads and immediately heads toward the first barrier inside the while loop.

Worker threads roll dice.
    Each worker thread rolls its dice and waits at barrierRolledDice.

Main thread waits.
    The main thread also waits at barrierRolledDice until all workers finish rolling.

Main thread calculates the winner.
    After all threads hit barrierRolledDice, the main thread calculates the winner and updates the status array.

Worker threads check the winner.
    Worker threads wait at barrierCalculated while the main thread calculates.

Main thread announces results.
    After all threads hit barrierCalculated, workers print their results.

Main thread loops back to start the next round.

*/
