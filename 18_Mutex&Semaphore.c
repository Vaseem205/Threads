/*

MUTEX: We can't lock the sharedData in one thread and unlock it in another thread (itll work for sometime and breaks)
SEMAPHORE: MUTEX: We can lock the sharedData in one thread and unlock it in another thread.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


sem_t semaphore;
int fuel = 50;

void* t1Func(void* args){
    while(1){
        sem_wait(&semaphore);
        fuel += 50;
        printf("Fuel: %d\n", fuel);
    }
}

void* t2Func(void* args){
    while(1){
        sem_post(&semaphore);
        usleep(500000);
    }
}


int main(){
    pthread_t t1, t2;
    sem_init(&semaphore, 0, 1);
    if (pthread_create(&t1, NULL, &t1Func, NULL) != 0) {
        perror("Failed to create thread");
    }
    if (pthread_create(&t2, NULL, &t2Func, NULL) != 0) {
        perror("Failed to create thread");
    }


    if (pthread_join(t1, NULL) != 0) {
        perror("Failed to join thread");
    }
    if (pthread_join(t2, NULL) != 0) {
        perror("Failed to join thread");
    }

    sem_destroy(&semaphore);

    return 0;

}

/*
Fuel: 100
Fuel: 150
Fuel: 200
Fuel: 250
Fuel: 300
...
...
...

NOTE: Here, we one t1 decrements semapore and t2 increments it, hence no waste of CPU cycle and it wont break like mutex.

*/