// #include <stdio.h>
// #include <stdlib.h>
// #include <stdlib.h>
// #include <pthread.h>

// #include <unistd.h>



// typedef struct{
//     pthread_mutex_t mutex;
//     pthread_cond_t  cond;
//     int ready;
//     int value;
// }sharedData;

// void intializeSharedData(sharedData* data){

//     pthread_mutex_init(&data->mutex, NULL);
//     pthread_cond_init(&data->cond, NULL);
//     data->ready = 0;
//     data->value = 0;
// }


// void* producerFunc(void* arg){

//     sharedData* data;
//     for(int i = 0; i < 5; i++){

//         pthread_mutex_lock(&data->mutex);

//         data->ready = 1;
//         data->value = i+1;

//         pthread_cond_broadcast(&data->cond);

//         printf("Producer: Set the value = %d\n", data->value);

//         pthread_mutex_unlock(&data->mutex);
//     }

//     sleep(3);
//     return NULL;

    
// }

// void* consumerFunc(void* arg){

//     int id = *(int*)arg;
//     sharedData* data = ((sharedData**)arg)[1];
    
//     for(int i = 0; i < 5; i++) {  // Consume all 5 values
//         pthread_mutex_lock(&data->mutex);
        
//         while(!data->ready) {
//             printf("Consumer[%d] is waiting\n", id);
//             pthread_cond_wait(&data->cond, &data->mutex);
//         }
        
//         printf("Consumer[%d] received value: %d\n", id, data->value);
        
//         // Only the last consumer should reset the ready flag
//         if(id == 3) {
//             data->ready = 0;
//         }
        
//         pthread_mutex_unlock(&data->mutex);
//     }

//     free(arg);
//     return NULL;
// }

// void deIntializeSharedData(sharedData* data){

//     pthread_mutex_destroy(&data->mutex);
//     pthread_cond_destroy(&data->cond);
//     data->ready = 0;
//     data->value = 0;
// }



// int main(){

//     sharedData data;

//     pthread_t producer;
//     pthread_t consumer[4];

//     intializeSharedData(&data);

    

//     pthread_create(&producer, NULL, &producerFunc, &data);

//     for(int i = 0; i < 4; i++){
//         void** args = malloc(2 * sizeof(void*));
//         int* id = malloc(sizeof(int));
//         *id = i;
//         args[0] = id;
//         args[1] = &data;
//         pthread_create(&consumer[i], NULL, &consumerFunc, args);
//     }


//     pthread_join(producer, NULL);
//     for(int i = 0; i < 4; i++){
//         pthread_join(consumer[i], NULL);
//     }

//     deIntializeSharedData(&data);


//     return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <unistd.h>

// typedef struct {
//     pthread_mutex_t mutex;
//     pthread_cond_t  cond;
//     int ready;
//     int value;
// } sharedData;

// void initializeSharedData(sharedData* data) {
//     pthread_mutex_init(&data->mutex, NULL);
//     pthread_cond_init(&data->cond, NULL);
//     data->ready = 0;
//     data->value = 0;
// }

// void* producerFunc(void* arg) {
//     sharedData* data = (sharedData*)arg;
    
//     for(int i = 0; i < 5; i++) {
//         pthread_mutex_lock(&data->mutex);
        
//         data->ready = 1;
//         data->value = i + 1;
        
//         printf("Producer: Set the value = %d\n", data->value);
//         pthread_cond_broadcast(&data->cond);
        
//         pthread_mutex_unlock(&data->mutex);
//         sleep(3); // Give consumers time to process
//     }
    
//     return NULL;
// }

// void* consumerFunc(void* arg) {
//     int id = *(int*)arg;
//     // printf("id: %d\n", id);
//     sharedData* data = ((sharedData**)arg)[1];
    
//     // for(int i = 0; i < 5; i++) {  // Consume all 5 values
//         pthread_mutex_lock(&data->mutex);
        
//         while(!data->ready) {
//             printf("Consumer[%d] is waiting\n", id);
//             pthread_cond_wait(&data->cond, &data->mutex);
//         }
        
//         printf("Consumer[%d] received value: %d\n", id, data->value);
        
//         // Only the last consumer should reset the ready flag
//         if(id == 3) {
//             data->ready = 0;
//         }
        
//         pthread_mutex_unlock(&data->mutex);
//     // }
    
//     free(arg);
//     return NULL;
// }

// void deinitializeSharedData(sharedData* data) {
//     pthread_mutex_destroy(&data->mutex);
//     pthread_cond_destroy(&data->cond);
//     data->ready = 0;
//     data->value = 0;
// }

// int main() {
//     sharedData data;
//     pthread_t producer;
//     pthread_t consumer[4];
    
//     initializeSharedData(&data);
    
//     pthread_create(&producer, NULL, &producerFunc, &data);
    
//     for(int i = 0; i < 4; i++) {
//         // Allocate space for both id and data pointer
//         void** args = malloc(2 * sizeof(void*));
//         int* id = malloc(sizeof(int));
//         *id = i;
//         args[0] = *id;
//         args[1] = &data;
//         pthread_create(&consumer[i], NULL, &consumerFunc, args);
//     }
    
//     pthread_join(producer, NULL);
//     for(int i = 0; i < 4; i++) {
//         pthread_join(consumer[i], NULL);
//     }
    
//     deinitializeSharedData(&data);
    
//     return 0;
// }

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;
int fuel = 0;
int ready = 0;

void* fuel_filling(void* arg) {
    for (int i = 0; i < 7; i++) {
        pthread_mutex_lock(&mutexFuel);
        fuel += 30;
        // ready = 1;
        printf("Filled fuel in petrol station........................ %d\n", fuel);
        pthread_mutex_unlock(&mutexFuel);
        pthread_cond_broadcast(&condFuel);
        sleep(1);
    }
}

void* car(void* arg) {
    pthread_mutex_lock(&mutexFuel);

    int a = *(int *)arg;

    // printf("a: %d\n", a);

    while (fuel < 40) {
        printf("----------------------------------------------------\n");
        printf("No fuel. Car is waiting to get it filled...\n");
        printf("----------------------------------------------------\n");
        pthread_cond_wait(&condFuel, &mutexFuel);
        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion\n");
        
    }
    fuel -= 40;
    // ready = 0;
    printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
    printf("Got fuel inside car. Now left fuel in petrol station: %d\n", fuel);
    printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
    
    pthread_mutex_unlock(&mutexFuel);
}

int main(int argc, char* argv[]) {
    pthread_t producer;
    pthread_t th[5];

    pthread_mutex_init(&mutexFuel, NULL);
    pthread_cond_init(&condFuel, NULL);

    pthread_create(&producer, NULL, &fuel_filling, NULL);

    for (int i = 0; i < 5; i++){
        int* a = malloc(sizeof(int));
        *a = i;
        pthread_create(&th[i], NULL, &car, a);
    }

    pthread_join(producer, NULL);
    for (int i = 0; i < 3; i++) {
        pthread_join(th[i], NULL);
    }

    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuel);
    return 0;
}

/*

Filled fuel in petrol station........................ 30
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
Filled fuel in petrol station........................ 60
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
|||||||||||||||||||||||||||||||||||||||||||||||||||||
Got fuel inside car. Now left fuel in petrol station: 20
|||||||||||||||||||||||||||||||||||||||||||||||||||||
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
Filled fuel in petrol station........................ 50
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
|||||||||||||||||||||||||||||||||||||||||||||||||||||
Got fuel inside car. Now left fuel in petrol station: 10
|||||||||||||||||||||||||||||||||||||||||||||||||||||
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
Filled fuel in petrol station........................ 40
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
|||||||||||||||||||||||||||||||||||||||||||||||||||||
Got fuel inside car. Now left fuel in petrol station: 0
|||||||||||||||||||||||||||||||||||||||||||||||||||||
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
Filled fuel in petrol station........................ 30
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
Filled fuel in petrol station........................ 60
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
|||||||||||||||||||||||||||||||||||||||||||||||||||||
Got fuel inside car. Now left fuel in petrol station: 20
|||||||||||||||||||||||||||||||||||||||||||||||||||||
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
----------------------------------------------------
No fuel. Car is waiting to get it filled...
----------------------------------------------------
Filled fuel in petrol station........................ 50
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!calling petrol statiion
|||||||||||||||||||||||||||||||||||||||||||||||||||||
Got fuel inside car. Now left fuel in petrol station: 10
|||||||||||||||||||||||||||||||||||||||||||||||||||||


*/

