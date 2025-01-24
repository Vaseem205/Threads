
/*

Key Requirements:
- Implement a thread pool with a fixed number of threads
- Create a task queue to manage incoming tasks
- Enable thread-safe task submission and execution
- Minimize CPU idle time and resource wastage
- Allow threads to wait when no tasks are available
- Support concurrent execution of multiple tasks


*/



#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define THREAD_NUM 4

typedef struct Task {
    int a, b;
} Task;

Task taskQueue[256];
int taskCount = 0;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

void executeTask(Task* task) {
    usleep(50000);
    int result = task->a + task->b;
    printf("The sum of %d and %d is %d\n", task->a, task->b, result);
}

void submitTask(Task task) {
    pthread_mutex_lock(&mutexQueue);
    taskQueue[taskCount] = task;
    taskCount++;
    pthread_mutex_unlock(&mutexQueue);
    pthread_cond_signal(&condQueue);
}

// 1 2 3 4 5
// 2 3 4 5

void* startThread(void* args) {
    while (1) {
        Task task;

        pthread_mutex_lock(&mutexQueue);
        while (taskCount == 0) {
            pthread_cond_wait(&condQueue, &mutexQueue);
        }

        task = taskQueue[0];
        int i;
        for (i = 0; i < taskCount - 1; i++) {
            taskQueue[i] = taskQueue[i + 1];
        }
        taskCount--;
        pthread_mutex_unlock(&mutexQueue);
        executeTask(&task);
    }
}

int main(int argc, char* argv[]) {
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexQueue, NULL);
    pthread_cond_init(&condQueue, NULL);
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_create(&th[i], NULL, &startThread, NULL) != 0) {
            perror("Failed to create the thread");
        }
    }

    srand(time(NULL));
    for (i = 0; i < 100; i++) {
        Task t = {
            .a = rand() % 100,
            .b = rand() % 100
        };
        submitTask(t);
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join the thread");
        }
    }
    pthread_mutex_destroy(&mutexQueue);
    pthread_cond_destroy(&condQueue);
    return 0;
}

/*

OUTPUT:

The sum of 69 and 15 is 84
The sum of 89 and 21 is 110
The sum of 8 and 67 is 75
The sum of 47 and 9 is 56
The sum of 70 and 97 is 167
...
...
...
100 lines of this type




FLOW:

1. Initialization Phase:
- Create mutex and condition variable
- Initialize thread pool with THREAD_NUM (4) threads
- Each thread runs startThread function indefinitely

2. Task Submission Process:
- Main thread generates tasks (100 random number pair tasks)
- submitTask function:
    -> Locks mutex
    -> Adds task to taskQueue
    -> Increments taskCount
    -> Unlocks mutex
    -> Signals a waiting thread via condition variable

3. Thread Execution Workflow:
- Threads continuously run in startThread function
- When taskCount is zero, threads wait on condition variable
- When signaled, thread:
    -> Locks mutex
    -> Removes first task from queue
    -> Shifts remaining tasks left
    -> Decrements taskCount
    -> Unlocks mutex
    -> Executes task via executeTask

4. Task Execution:
- executeTask simulates work by sleeping 50ms
- Adds two numbers
- Prints result


5. Synchronization Mechanisms:
- Mutex prevents concurrent queue modifications
- Condition variable manages thread waiting/signaling



NOTE: More threads leads to more fast execution

*/