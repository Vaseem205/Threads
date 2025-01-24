/*

Problem Statement:

Managing a shared buffer accessed by multiple threads, with the following key challenges:

Shared Memory Access: Prevent race conditions when multiple producers and consumers access the buffer simultaneously.
Buffer Overflow: Handle situations where producers are adding items faster than consumers can remove them, potentially filling the buffer.
Buffer Underflow: Manage scenarios where consumers attempt to remove items from an empty buffer.

The core objective is to create a synchronization mechanism that allows:

Safe concurrent access to the shared buffer
Blocking producers when the buffer is full
Blocking consumers when the buffer is empty
Efficient thread communication and resource utilization

*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <string.h>

#define THREAD_NUM 8

sem_t semEmpty;
sem_t semFull;

pthread_mutex_t mutexBuffer;

int buffer[10];
int count = 0;

void* producer(void* args) {
    while (1) {
        // Produce
        int x = rand() % 100;
        sleep(1);

        // Add to the buffer
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
        buffer[count] = x;
        printf("PRODUCER:\tbuffer[%d] = %d\n",count, x);
        count++;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }
}

void* consumer(void* args) {
    while (1) {
        int y;

        // Remove from the buffer
        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);
        y = buffer[count - 1];

        // Consume
        printf("CONSUMER:\tGot %d\t\tfrom buffer[%d]\n", y, count-1);
        
        count--;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);

        
        
        
        sleep(1);
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, 10);
    sem_init(&semFull, 0, 0);
    int i;

    // 4 producer and 4 cosumer thdread will get created

    for (i = 0; i < THREAD_NUM; i++) {
        if (i % 2 == 0) {
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &consumer, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
    }
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    return 0;
}

/*

PRODUCER:	buffer[0] = 89
PRODUCER:	buffer[1] = 14
CONSUMER:	Got 14		from buffer[1]
CONSUMER:	Got 89		from buffer[0]
PRODUCER:	buffer[0] = 74
CONSUMER:	Got 74		from buffer[0]
PRODUCER:	buffer[0] = 51
CONSUMER:	Got 51		from buffer[0]
<sleeps for 1 sec>
...
...
...


NOTE:

4 producers get in parallelly and update the buffer (at that time sem_wait(&semEmpty); will do 4 semEmpty--),
those threads then do semFull++ (remember we initialized it with 0). as semFull semaphore has 4 in it, it'll
go and execute consumer thread. 4 threads enter consumer thread and does semFull--, by the end of it, they'll
increment semEmpty++ 4 times.

And, mutex is used just to make sure that, while producer is updating something in buffer, the consumer thread
should not consume anything from buffer.

*/