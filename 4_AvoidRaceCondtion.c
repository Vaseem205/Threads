/*

Avoiding RACE Condition using inbuilt MUTEX fucntion in pthread.h

*/




#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int x = 0;

pthread_mutex_t mutex;

void* thread1_Func(){
    for(int i = 0; i<1000000; i++){
        pthread_mutex_lock(&mutex);
        x++;
        pthread_mutex_unlock(&mutex);
    }    
}

int main(){
    
    pthread_t t1, t2;

    pthread_mutex_init(&mutex, NULL);

    if(pthread_create(&t1, NULL, &thread1_Func, NULL) != 0){
        return 1;
    }
    if(pthread_create(&t2, NULL, &thread1_Func, NULL) != 0){
        return 2;
    }
    if(pthread_join(t1, NULL) != 0){                //Wait for thread t1 to finish execution
        return 3;
    }                     
    if(pthread_join(t2, NULL) != 0){                //Wait for thread t2 to finish execution
        return 4;
    }             

    pthread_mutex_destroy(&mutex);

    printf("in main: %d\n", x);

    return 0;
}

/*

OUTPUT:

in main: 2000000 

NO RACE Conditions 


*/
