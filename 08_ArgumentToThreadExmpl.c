/*

Thread_1 -> sums of 0-4 index elements
Thread_2 -> sums of 5-9 index elements

In main()

Sum = Thread_1_Sum + Thread_2_Sum.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int primes[10] = {2, 3 , 5 , 7, 11, 13, 17, 19, 23, 29};

void* func1(void* val){


    int index = *(int *)val;
    int sum = 0;

    for(int i = 0; i<5; i++){
        sum += primes[index+i];
    }

    printf("thread[%d] sum: %d\n", index/5, sum);

    *(int *)val = sum;

    return val;
}

int main(){

    pthread_t t[2];

    for(int i = 0; i<2; i++){
        int* a = malloc(sizeof(int));
        *a = i*5;
        printf("address of a : %p | value of a passed to thread as arg: %d\n", a, *a);
        if(pthread_create(&t[i], NULL, &func1, a)!=0){
            perror("pacick");
            return 1;
        }
    }

    int Sum = 0;

    for(int i = 0; i<2; i++){
        int *r;
        if(pthread_join(t[i], (void **)&r)!=0){
            return 2;
        }
        
        Sum += *r;
        free(r);
    }

    printf("Sum: %d\n", Sum);
    return 0;
}



/*

OUTPUT:
address of a : 0x5ff17cb6c2a0 | value of a passed to thread as arg: 0
address of a : 0x5ff17cb6c7f0 | value of a passed to thread as arg: 5
thread[0] sum: 28
thread[1] sum: 101
Sum: 129


*/