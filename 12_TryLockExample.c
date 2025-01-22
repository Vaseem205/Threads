#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


pthread_mutex_t multipleMutex[4];
int sharedData[4] = {100, 100, 100, 100};


void* threadFunc(void* args) {
    // As there are 4 sharedData we should iterate through each data, hold lock, do some stuff, release lock
    // We have to wait if all 4 sharedData has been locked by other threads
    for (int i = 0; i < 4; i++) {
        if (pthread_mutex_trylock(&multipleMutex[i]) == 0) {
            printf("\t\t\t\t\t\t\t  passed sharedData[%d]: %d\n",i, sharedData[i]);
            int decrementValue = (rand() % 30);
            if (sharedData[i] - decrementValue < 0) {
                printf("No value left in sharedData[%d] to decrement\n", i);
            }else{
                sharedData[i] -= decrementValue;
                usleep(500000);
                printf("Thread [%ld]\tdeceremented [%d]\t current sharedData[%d]: %d\t by holding multipleMutex[%d]\n", pthread_self(), decrementValue ,i, sharedData[i], i);
            }
            pthread_mutex_unlock(&multipleMutex[i]);
            break;
        } else {
            if (i == 3) {
                //This if condition make sure that after completing 4 iteration of for loop, i.e. 4 threads, the other  threads should also get chance.
                // In order to do it we reset the i value
                printf("All sharedData is locked, waiting...\n");
                usleep(300000);
                i = 0;
            }
        }
    }
}


int main(int argc, char* argv[]) {
    srand(time(NULL));
    pthread_t th[10];
    for (int i = 0; i < 4; i++) {
        pthread_mutex_init(&multipleMutex[i], NULL);
    }
    for (int i = 0; i < 10; i++) {
        if (pthread_create(&th[i], NULL, &threadFunc, NULL) != 0) {
            perror("Failed to create thread");
        }
    }

    for (int i = 0; i < 10; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    for (int i = 0; i < 4; i++) {
        pthread_mutex_destroy(&multipleMutex[i]);
    }
    return 0;
}

/*

							                    passed sharedData[0]: 100
							                    passed sharedData[2]: 100
							                    passed sharedData[1]: 100
							                    passed sharedData[3]: 100
All sharedData is locked, waiting...
All sharedData is locked, waiting...
All sharedData is locked, waiting...
All sharedData is locked, waiting...
All sharedData is locked, waiting...
All sharedData is locked, waiting...
All sharedData is locked, waiting...
All sharedData is locked, waiting...
All sharedData is locked, waiting...
All sharedData is locked, waiting...
All sharedData is locked, waiting...
All sharedData is locked, waiting...
Thread [124770251179584]	deceremented [18]	 current sharedData[0]: 82	 by holding multipleMutex[0]
Thread [124770230208064]	deceremented [20]	 current sharedData[2]: 80	 by holding multipleMutex[2]
Thread [124770219722304]	deceremented [23]	 current sharedData[3]: 77	 by holding multipleMutex[3]
Thread [124770240693824]	deceremented [11]	 current sharedData[1]: 89	 by holding multipleMutex[1]
							                    passed sharedData[1]: 89
							                    passed sharedData[2]: 80
							                    passed sharedData[3]: 77
All sharedData is locked, waiting...
All sharedData is locked, waiting...
All sharedData is locked, waiting...
All sharedData is locked, waiting...
All sharedData is locked, waiting...
All sharedData is locked, waiting...
Thread [124770129544768]	deceremented [23]	 current sharedData[2]: 57	 by holding multipleMutex[2]
Thread [124770119059008]	deceremented [13]	 current sharedData[3]: 64	 by holding multipleMutex[3]
Thread [124770140030528]	deceremented [12]	 current sharedData[1]: 77	 by holding multipleMutex[1]
							                    passed sharedData[1]: 77
							                    passed sharedData[2]: 57
							                    passed sharedData[3]: 64
Thread [124770098087488]	deceremented [8]	 current sharedData[1]: 69	 by holding multipleMutex[1]
Thread [124770087601728]	deceremented [23]	 current sharedData[3]: 41	 by holding multipleMutex[3]
Thread [124770108573248]	deceremented [5]	 current sharedData[2]: 52	 by holding multipleMutex[2]


*/