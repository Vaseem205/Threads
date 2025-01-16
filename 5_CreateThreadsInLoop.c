#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// pthread_mutex_t mutex;

// int x = 0;

// void* thread_func1(){
//     for(int i = 0; i<1000000; i++){
//         pthread_mutex_lock(&mutex);
//         x++;
//         pthread_mutex_unlock(&mutex);
//     }
// }

// int main(){

//     pthread_t p[4];

//     pthread_mutex_init(&mutex, NULL);

//     int size_of_p = sizeof(p)/sizeof(pthread_t);

//     for(int i=0; i<size_of_p; i++){
//         if(pthread_create(&p[i], NULL, &thread_func1, NULL) != 0){
//             printf("Failed to create the thread");
//             return 1;
//         }
//         printf("Thread_%d has created\n", i);
//         if(pthread_join(p[i], NULL)!=0){
//             return 2;
//         }
//         printf("Thread_%d has finished execution\n", i);

//     }
//     printf("value of x: %d\n", x);
//     return 0;
// }

/*

OUTPUT:

Thread_0 has created
Thread_0 has finished execution
Thread_1 has created
Thread_1 has finished execution
Thread_2 has created
Thread_2 has finished execution
Thread_3 has created
Thread_3 has finished execution
value of x: 4000000



It is observed that, threads are getting started and finished its execution and then the subsequest threads are being executed.
But, as its multi-threaded we expect it to do execute in parallel and output as follows:
Thread_0 has created
Thread_1 has created
Thread_2 has created
Thread_3 has created
Thread_0 has finished execution
Thread_1 has finished execution
Thread_2 has finished execution
Thread_3 has finished execution
value of x: 4000000

But the contradictory behaviour was observed because of the way for loop has been implmented. Below code solve that issue.

*/


pthread_mutex_t mutex;

int x = 0;

void* thread_func1(){
    for(int i = 0; i<1000000; i++){
        pthread_mutex_lock(&mutex);
        x++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(){

    pthread_t p[4];

    pthread_mutex_init(&mutex, NULL);

    int size_of_p = sizeof(p)/sizeof(pthread_t);

    for(int i=0; i<size_of_p; i++){
        if(pthread_create(&p[i], NULL, &thread_func1, NULL) != 0){
            printf("Failed to create the thread");
            return 1;
        }
        printf("Thread_%d has created\n", i);
    }

    for(int i=0; i<size_of_p; i++){
        if(pthread_join(p[i], NULL)!=0){
            return 2;
        }
        printf("Thread_%d has finished execution\n", i);
    }

    pthread_mutex_destroy(&mutex);
    
    printf("value of x: %d\n", x);

    return 0;
}

/*

OUTPUT:
Thread_0 has created
Thread_1 has created
Thread_2 has created
Thread_3 has created
Thread_0 has finished execution
Thread_1 has finished execution
Thread_2 has finished execution
Thread_3 has finished execution
value of x: 4000000


Here, the threads are executed in parallel.

*/