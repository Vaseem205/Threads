#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


// trying normal mutex_lock

/*
pthread_mutex_t mutex;

void* func1(){
    pthread_mutex_lock(&mutex);
    sleep(1);
    printf("Lock Acquired\n");
    pthread_mutex_unlock(&mutex);
}

int main(){
    
    pthread_t t[4];

    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < 4; i++){
        if(pthread_create(&t[i], NULL, &func1, NULL) != 0){
            perror("Failed to create thread");
        }
    }

    for(int i = 0; i < 4; i++){
        if(pthread_join(t[i], NULL) != 0){
            perror("Failed to join thread");
        }
    }


    return 0;
}

*/

/*

OUTPUT:

Lock Acquired
Lock Acquired
Lock Acquired
Lock Acquired

NOTE:
This things got printed, with 1 second delay. Why?
One thread will acquire the lock, print it and release the lock. Till then other threads will wait for the lock to get released.

*/



pthread_mutex_t mutex;

void* func1(){

    if(pthread_mutex_trylock(&mutex)==0){
        sleep(1);
        printf("Lock Acquired\n");
        pthread_mutex_unlock(&mutex);
    }else{
        printf("Could not acquire lock\n");
    }
    
}

int main(){
    
    pthread_t t[4];

    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < 4; i++){
        if(pthread_create(&t[i], NULL, &func1, NULL) != 0){
            perror("Failed to create thread");
        }
    }

    for(int i = 0; i < 4; i++){
        if(pthread_join(t[i], NULL) != 0){
            perror("Failed to join thread");
        }
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}


/*

OUTPUT:

Could not acquire lock
Could not acquire lock
Could not acquire lock
Lock Acquired


NOTE:
The difference between mutex_lock and mutex_trylock is:
mutex_lock waits till it acquires the lock (which makes it slow)
mutex_trylock will not wait to acquire the lock (this makes it faster)

*/



