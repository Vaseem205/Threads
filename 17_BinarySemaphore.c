/*

Normal semaphore= (Counting Semaphore) could have the value from: -∞ to +∞
That means when you do the decrement or increment using sem_wait(&semaphore) and sem_post(&semaphore) resp,
the value of that used &semaphore could vary in this range (-∞ to +∞)

But in Binary Semaphore only have 2 values: 0 & 1

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphore;

int* fuel;

void* thread1Func(void* args){

    *fuel += 50;
    
    sem_post(&semaphore);
}


int main(){

    pthread_t thread1;
    sem_init(&semaphore, 0 , 0);
    fuel = malloc(sizeof(int));
    *fuel = 50;

    if (pthread_create(&thread1, NULL, &thread1Func, NULL) != 0) {
        perror("Failed to create thread");
    }

    sem_wait(&semaphore);
    printf("Fuel: %d\n", *fuel);
    free(fuel);

    if (pthread_join(thread1, NULL) != 0) {
        perror("Failed to join thread");
    }
    sem_destroy(&semaphore);

    return 0;
}

/*

Fuel: 100


NOTE:

1. We are intializing semaphore with value 0 {sem_init(&semaphore, 0 , 0);}
2. thread1 gets created and calls thread1Func
3. As semaphore is init with 0 we cant have sem_wait in the thread1Func routine, cuz the thread only get executed if the semaphore is >0
4. In this code, we execute and then increment semaphore by doing: sem_post(&semaphore);
5. Now in main, we see {sem_wait(&semaphore)}. This wont let further executiong to happen, until the semaphore gets incremented and get >0.
6. As we increment it in routine, sem_wait will pass and we get the altered fuel value 100.

*/