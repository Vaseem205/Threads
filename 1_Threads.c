#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


/*

void* func(){
    printf("Inside thread\n");
    sleep(3);
    printf("Exit thread\n");
}
int main(){

    pthread_t t1, t2;

    pthread_create(&t1, NULL, &func, NULL);
    pthread_create(&t2, NULL, &func, NULL);
    pthread_join(t1, NULL);                     // Wait for thread t1 to finish execution
    pthread_join(t2, NULL);                     // Wait for thread t2 to finish execution

    return 0;
}


*/

/*
Output:

Inside thread
Inside thread
<sleep for 3 seconds>
Exit thread
Exit thread

NOTE: Both the threads executes in parallel. Otherwise the output would have been as follows:
Inside thread
<sleep for 3 seconds>
Exit thread
Inside thread
<sleep for 3 seconds>
Exit thread

*/

//With error checking:

void* func(){
    printf("Inside thread\n");
    sleep(3);
    printf("Exit thread\n");
}
int main(){

    pthread_t t1, t2;

    if(pthread_create(&t1, NULL, &func, NULL) != 0){
        return 1;
    }
    if(pthread_create(&t2, NULL, &func, NULL) != 0){
        return 2;
    }
    if(pthread_join(t1, NULL) != 0){                //Wait for thread t1 to finish execution
        return 3;
    }                     
    if(pthread_join(t2, NULL) != 0){                //Wait for thread t2 to finish execution
        return 4;
    }                     
    return 0;
}