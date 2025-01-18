#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


void* func1(){
    
}


int main(){

    pthread_t t[3];

    int size = sizeof(t)/sizeof(pthread_t);

    for(int i = 0; i<size; i++){
        if(pthread_create(t[i], NULL, &func1, NULL)!=0){
            return 1;
        }
    }

    for(int i = 0; i<size; i++){
        if(pthread_join(t[i], NULL)!=0){
            return 2;
        }
    }


    return 0;
}