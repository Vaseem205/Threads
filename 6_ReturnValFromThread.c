/*

Fucntion call: (pthread_join(t1, NULL)!=0)

The second parameter of this function is configured to store the return value of the thread.
It is a double pointer of type void. 

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


/*
void* dice(){


    int value = (rand()%6)+1;                           // generate rand_no bw 1-6
    int* result = malloc(sizeof(int));
    *result = value;
    printf("Thread result address: \t %p\n", result);                             // address of result
    return (void *)result;                              // typecasting cuz return_type is void*
}

int main(){

    pthread_t t1;
    int* res;
    srand(time(NULL));                                  // specifying the seed, otherwise the random func generates same random values each time

    if(pthread_create(&t1, NULL, &dice, NULL)!=0){
        return 1;
    }

    if(pthread_join(t1, (void**)&res)!=0){              //passing the pointer "res" and typecastig it, cuz, thats how it expects tge second parameter to be                 
        return 2;
    }
    printf("main res address: \t %p\n", res);                                // address of res and result (from dice()) should be same
    printf("%d\n", *res);

    free(res);
    return 0;
}

*/

/*

OUTPUT:
Thread result address: 	 0x7e1790000b70
main res address: 	 0x7e1790000b70
3

But, we are doing malloc() in dice() and free() of same pointer in main(). This is not a good practice

*/


/*

printing 6 random numbers

*/

void* dice(){


    int value = (rand()%6)+1;                           // generate rand_no bw 1-6
    int* result = malloc(sizeof(int));
    *result = value;
    printf("Thread result address: \t %p\n", result);   // address of result
    return (void *)result;                              // typecasting cuz return_type is void*
}


int main(){

    pthread_t t[6];
    int* res;
    srand(time(NULL));       

    for(int i =0; i<6; i++){
        if(pthread_create(&t[i], NULL, &dice, NULL)!=0){
            return 1;
        }
    }
    
    for(int i = 0; i<6; i++){
        if(pthread_join(t[i], (void**)&res)!=0){       
            return 2;
        }
        printf("main res address: \t %p\n", res);  
        printf("%d\n", *res);
        free(res);
    }
    
    return 0;
}