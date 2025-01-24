
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


int x = 0;

void* thread1_Func(){
    for(int i = 0; i<100000; i++){
        x++;
    }    
}

int main(){
    
    pthread_t t1, t2;

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

    printf("in main: %d\n", x);

    return 0;
}

/*

OUTPUT:

in main: 124350

BUT, for 10000 iteration in thread1_Func(), the output is 20000. Why is this happening?

Ans: RACE Condition.

Checkout Dekker's Algorithm to avoid this RACE Condition (https://youtu.be/MqnpIwN7dz0?si=1Z6K9qUWx-S2eNLA).

*/



