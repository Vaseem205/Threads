#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define THREAD_NUM 4

typedef struct Task {
    int (*taskFunction)(int, int);
    int arg1, arg2;
} Task;

Task taskQueue[256];
int taskCount = 0;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

int sum(int a, int b) {
    usleep(50000);
    int sum = a + b;
    printf("Sum of %d and %d is %d\n", a, b, sum);
    return sum;
}

int product(int a, int b) {
    usleep(50000);
    int prod = a * b;
    printf("Product of %d and %d is %d\n", a, b, prod);
    return prod;
}

void executeTask(Task* task) {
    int result = task->taskFunction(task->arg1, task->arg2);
    printf("reslt of executeTask: %d\n", result);
}

void submitTask(Task task) {
    pthread_mutex_lock(&mutexQueue);
    taskQueue[taskCount] = task;
    taskCount++;
    pthread_mutex_unlock(&mutexQueue);
    pthread_cond_signal(&condQueue);
}

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
    for (i = 0; i < 10; i++) {
        Task t = {
            .taskFunction = i % 2 == 0 ? &sum : &product, // Half of them will call sum and rest half will call product.
            .arg1 = rand() % 100,
            .arg2 = rand() % 100
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
Product of 59 and 53 is 3127
Product of 54 and 10 is 540
Sum of 83 and 36 is 119
Sum of 14 and 87 is 101
Sum of 45 and 65 is 110
Product of 21 and 98 is 2058
Product of 44 and 49 is 2156
Sum of 99 and 83 is 182
Sum of 73 and 77 is 150
Product of 55 and 50 is 2750


NOTE: half of the task were sum and rest half are sum


OUTPUT of current code:
Sum of 47 and 2 is 49
reslt of executeTask: 49
Product of 76 and 15 is 1140
reslt of executeTask: 1140
Product of 88 and 85 is 7480
reslt of executeTask: 7480
Sum of 0 and 51 is 51
reslt of executeTask: 51
Sum of 11 and 19 is 30
reslt of executeTask: 30
Product of 37 and 36 is 1332
reslt of executeTask: 1332
Product of 19 and 69 is 1311
reslt of executeTask: 1311
Sum of 72 and 99 is 171
reslt of executeTask: 171
Product of 64 and 40 is 2560
reslt of executeTask: 2560
Sum of 83 and 4 is 87
reslt of executeTask: 87




FLOW:

Flow of Generalized Thread Pool Implementation:

1. Task Structure Enhancement:
- Task now includes function pointer (taskFunction)
- Supports multiple task functions (sum, product)
- Allows dynamic task definition using ternary operator {.taskFunction = i % 2 == 0 ? &sum : &product}


2. Task Submission Process:
- Main thread generates 10 tasks
- Alternates between sum and product functions
- Uses random numbers as arguments
- Submits tasks to queue via submitTask()


3. Thread Execution Workflow:
- Threads wait on condition variable in startThread()-> {pthread_cond_wait(&condQueue, &mutexQueue)} while taskCount==0
- When taskCount incremented and signaled (pthread_cond_signal(&condQueue)), the thread does the follwing:
    -> Locks mutex
    -> Removes first task from queue
    -> Shifts remaining tasks left
    -> Decrements taskCount
    -> Unlocks mutex
    -> Executes task via executeTask

4. Task Execution Strategy:
- executeTask() calls task's function pointer
- Passes pre-defined arguments (happened in main thread)
- Functions simulate work by sleeping
- Prints task-specific results


*/