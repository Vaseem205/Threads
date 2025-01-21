/*

Conditional Variables work with Mutexes:

We always use conditional variables with mutexes to protect shared data
The mutex ensures exclusive access to shared data
The condition variable allows threads to wait for specific conditions


Main Components:

pthread_cond_t: The condition variable type
pthread_cond_wait(): Makes a thread wait for a condition
pthread_cond_signal(): Signals waiting threads that a condition has changed


Producer-Consumer Pattern:

The producer sets data and signals it's ready
The consumer waits for data to be ready before processing
This prevents the consumer from reading data before it's ready


Important Points:

Always check conditions in a while loop (not if) to handle spurious wakeups
The mutex must be locked when calling pthread_cond_wait()
pthread_cond_wait() automatically releases the mutex while waiting and reacquires it when woken up

*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
typedef struct{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int ready;
    int value;
}sharedData;

void initializeSharedData(sharedData* data){
    pthread_mutex_init(&data->mutex, NULL);
    pthread_cond_init(&data->cond, NULL);
    data->ready = 0;
    data->value = 0;
}

void* producerFunc(void* arg){
    sharedData* data = (sharedData *) arg;

    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&data->mutex);
        
        data->value = i + 1;
        data->ready = 1;
        printf("Producer: Set value to %d\n", data->value);
        
        pthread_cond_signal(&data->cond);
        
        pthread_mutex_unlock(&data->mutex);
        
        sleep(1);
    }


}

void* consumerFunc(void* arg){

    sharedData* data = (sharedData *) arg;

    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&data->mutex);
        
        while (!data->ready) {
            printf("Consumer: Waiting for data...\n");
            pthread_cond_wait(&data->cond, &data->mutex);
        }
        
        printf("Consumer: Got value %d\n", data->value);
        data->ready = 0; // Reset ready flag
        
        pthread_mutex_unlock(&data->mutex);
    }
    
    return NULL;

}

void un_initializeSharedData(sharedData* data){
    pthread_mutex_destroy(&data->mutex);
    pthread_cond_destroy(&data->cond);
    data->ready = 0;
    data->value = 0;
}

int main(){
    sharedData data;
    pthread_t producer, consumer;

    initializeSharedData(&data);

    pthread_create(&producer, NULL, &producerFunc, &data);
    pthread_create(&consumer, NULL, &consumerFunc, &data);


    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);


    un_initializeSharedData(&data);

    return 0;
}

*/

/*

OUTPUT:
Producer: Set value to 1
Consumer: Got value 1
Consumer: Waiting for data...
Producer: Set value to 2
Consumer: Got value 2
Consumer: Waiting for data...
Producer: Set value to 3
Consumer: Got value 3
Consumer: Waiting for data...
Producer: Set value to 4
Consumer: Got value 4
Consumer: Waiting for data...
Producer: Set value to 5
Consumer: Got value 5



*/

typedef struct{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int ready;
    int value;
}sharedData;

void initSharedData(sharedData* data){
    pthread_mutex_init(&data->mutex, NULL);
    pthread_cond_init(&data->cond, NULL);
    data->ready = 0;
    data->value = 0;
}

void* producerFunc(void* arg){

    sharedData* data = (sharedData *)arg;

    pthread_mutex_lock(&data->mutex);

    data->ready = 1;
    data->value = 1;

    printf("producer set the value: %d\n", data->value );

    pthread_cond_signal(&data->cond);

    pthread_mutex_unlock(&data->mutex);

    sleep(10);

}

void* consumerFunc(void* args){

    sharedData* data = (sharedData *)args;

    pthread_mutex_lock(&data->mutex);

    while(!data->ready){    
        printf("Consumer_1 is waiting\n");
        pthread_cond_wait(&data->cond, &data->mutex);
    }

    printf("consumer_1 got the value: %d\n", data->value);

    data->ready = 0;

    pthread_mutex_unlock(&data->mutex);

    return NULL;

}

void* consumerFunc2(void* args){

    sharedData* data = (sharedData *)args;

    pthread_mutex_lock(&data->mutex);

    while(!data->ready){    
        printf("Consumer_2 is waiting\n");
        pthread_cond_wait(&data->cond, &data->mutex);
    }

    printf("consumer_2 got the value: %d\n", data->value);

    data->ready = 0;

    pthread_mutex_unlock(&data->mutex);

    return NULL;

}

void deInitSharedData(sharedData* data){
    pthread_mutex_destroy(&data->mutex);
    pthread_cond_destroy(&data->cond);
    data->ready = 0;
    data->value = 0;
}

int main(){
    sharedData data;
    pthread_t producer;
    pthread_t consumer1, consumer2;

    initSharedData(&data);

    pthread_create(&producer, NULL, &producerFunc, &data);
    pthread_create(&consumer1, NULL, &consumerFunc, &data);
    pthread_create(&consumer2, NULL, &consumerFunc2, &data);

    pthread_join(producer, NULL);
    pthread_join(consumer1, NULL);
    pthread_join(consumer2, NULL);

    deInitSharedData(&data);


    return 0;
}

/*

OUTPUT:
producer set the value: 1
consumer_1 got the value: 1
Consumer_2 is waiting


As, pthread_cond_signal() only singnals for 1 thread, the Consumer_2 will always be in waiting state.

*/


/*

Conditional Variables work with Mutexes:

We always use conditional variables with mutexes to protect shared data
The mutex ensures exclusive access to shared data
The condition variable allows threads to wait for specific conditions


Main Components:

pthread_cond_t: The condition variable type
pthread_cond_wait(): Makes a thread wait for a condition
pthread_cond_signal(): Signals waiting threads that a condition has changed


Producer-Consumer Pattern:

The producer sets data and signals it's ready
The consumer waits for data to be ready before processing
This prevents the consumer from reading data before it's ready


Important Points:

Always check conditions in a while loop (not if) to handle spurious wakeups
The mutex must be locked when calling pthread_cond_wait()
pthread_cond_wait() automatically releases the mutex while waiting and reacquires it when woken up

*/






