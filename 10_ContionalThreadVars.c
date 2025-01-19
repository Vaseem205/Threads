#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


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