/***********

A process can contain multiple threads.

Demonstatrtion:

1. Create 2 threads
2. Print the process id of it.

***********/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

/*

void* func(){
    printf("PID: %d\n", getpid());    
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

*/

/*

OUTPUT:
PID: 153430
PID: 153430

NOTE: Same PID for both the threads

*/


/***********

When we fork() a process, it created 2 process and also the child process duplicates the parent process memory.
Parent and Child Proces will have exclusive memory.

BUT, thats not the case in threads. Threads share the same memory space. 

Demonstration:
1. Create 2 threads
2. Modidy a variable in thread_1 and print it
3. Print the same variable in thread_2

***********/


int x = 2;

void* thread1_Func(){
    x++;                                        //sleep(2) is to make sure that the x is incremented before printing.
    sleep(2);
    printf("x in thread 1: %d\n", x);
}

void* thread2_Func(){
    sleep(2);                                   //sleep(2) is to make sure that the x is incremented before printing.
    printf("x in thread 2: %d\n", x);
}


int main(){
    
    pthread_t t1, t2;

    if(pthread_create(&t1, NULL, &thread1_Func, NULL) != 0){
        return 1;
    }
    if(pthread_create(&t2, NULL, &thread2_Func, NULL) != 0){
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

/*

OUTPUT:

x in thread 1: 3
x in thread 2: 3

NOTE: Both the threads modifies the same variable x. This is because threads share the same memory space.

*/










