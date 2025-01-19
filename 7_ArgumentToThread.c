/*

Print One Prime Number from one thread

Ex: A thread should pick up one index of array which contains prime number and print it.

*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


/*
int primes[10] = {2, 3 , 5 , 7, 11, 13, 17, 19, 23, 29};

void* func1(void* val){
    sleep(1);
    int index = *(int *)val;
    printf("primes[%d]: %d\n", index, primes[index]);
}

int main(){

    pthread_t t[10];
    int size = sizeof(t)/sizeof(pthread_t);

    for(int i = 0; i<10; i++){
        if(pthread_create(&t[i], NULL, &func1, &i)!=0){
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

*/

/*

OUTPUT:

primes[1]: 3
primes[2]: 5
primes[3]: 7
primes[4]: 11
primes[5]: 13
primes[6]: 17
primes[7]: 19
primes[8]: 23
primes[9]: 29
primes[10]: 0


Here you could see that, the first index value has been skipped.
And if i instroduce sleep(1) inside func1() just before everything else in teh func1() starts executing, this the the observed behaviour.
primes[10]: 0
primes[10]: 0
primes[10]: 0
primes[10]: 0
primes[10]: 0
primes[10]: 0
primes[10]: 0
primes[10]: 0
primes[10]: 0
primes[10]: 0

This is happening becuase, whenever the thread gets created using pthread_create, it'll go into the function func1(), but there it is sleeping for 1 sec.
In the 1 sec, all other 10 threads are being created, so the last value of present in &i is 10. At that time, th thread tries to printout the values prsent at index 10.

How to solve?

*/

/*

In the above code, each thread depended on single variable "i".
Changes in the value of "i" (by for loop) is affecting the thread.

What if, we pass the different memory address each time to the thread, which wont change. so the index we are passing will not be altered.

*/



int primes[10] = {2, 3 , 5 , 7, 11, 13, 17, 19, 23, 29};

void* func1(void* val){
    sleep(1);
    int index = *(int *)val;
    printf("primes[%d]: %d\n", index, primes[index]);
    free(val);                                           //freeing pointer a. (even if its void*, it does not matter as all ptrs are of same size)
}

int main(){

    pthread_t t[10];
    int size = sizeof(t)/sizeof(pthread_t);

    for(int i = 0; i<10; i++){
        int* a = malloc(sizeof(int));
        *a = i;
        printf("address of a : %p | value of a passed to thread as arg: %d\n", a, *a);
        if(pthread_create(&t[i], NULL, &func1, a)!=0){
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


/*

OUTPUT:

vaseem@vaseemakram:~/C_Programming/Threads$ ./7_ArgumentToThread 
address of a : 0x60b04db7e2a0 | value of a passed to thread as arg: 0
address of a : 0x60b04db7e7f0 | value of a passed to thread as arg: 1
address of a : 0x60b04db7e930 | value of a passed to thread as arg: 2
address of a : 0x60b04db7ea70 | value of a passed to thread as arg: 3
address of a : 0x60b04db7ebb0 | value of a passed to thread as arg: 4
address of a : 0x60b04db7ecf0 | value of a passed to thread as arg: 5
address of a : 0x60b04db7ee30 | value of a passed to thread as arg: 6
address of a : 0x60b04db7ef70 | value of a passed to thread as arg: 7
address of a : 0x60b04db7f0b0 | value of a passed to thread as arg: 8
address of a : 0x60b04db7f1f0 | value of a passed to thread as arg: 9
primes[0]: 2
primes[1]: 3
primes[2]: 5
primes[3]: 7
primes[4]: 11
primes[5]: 13
primes[6]: 17
primes[7]: 19
primes[8]: 23
primes[9]: 29


Here you can observe that address of a is different every time and that holds the index number.
And, nowhere in the code its(address) being altered. So the index value are safe in it. 

If you observe, we are free() the pointer a in func1(). Doign that in main() again messup the addresses


*/
















