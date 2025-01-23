/*

	        |--- s==0 --> wait
sem_wait----|
	        |--- s>0 ---> execute the below code and do s--
	 
sem_post ---> s++

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>


sem_t semaphore;

void* semFunc(void* args){
    int index = *(int *)args;
    free(args);
    sem_wait(&semaphore);
    sleep(1);
    printf("Thread[%d] got executed\n", index);
    sem_post(&semaphore);
}


int main(){
    pthread_t th[4];
    sem_init(&semaphore, 0, 2); // it'll make 2 threads run at once

    for(int i = 0; i<4; i++){
        int* a = malloc(sizeof(int));
        *a = i;
        if(pthread_create(&th[i], NULL, &semFunc, (void *)a)!=0){
            perror("Failed to create Thread");
        }
    }

    for(int i = 0; i<4; i++){
        if(pthread_join(th[i], NULL)!=0){
            perror("Failed to join Thread");
        }
    }

    sem_destroy(&semaphore);
    return 0;
}

/*

Thread[1] got executed
Thread[0] got executed
<slept for 1 sec>
Thread[3] got executed
Thread[2] got executed


NOTE:

here 2 threads got executed parallelly and sleep for 1 second after executing it.
Cuz of: sem_init(&semaphore, 0, 2);
*/