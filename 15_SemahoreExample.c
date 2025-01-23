/*

In a popular online multiplayer game, server capacity is limited. 
The game experiences high player demand, with significantly more gamers wanting to play 
simultaneously than the server can support. To manage this, a login queue system is needed that:

- Restricts total concurrent players to the server's maximum capacity
- Ensures fair access by making excess players wait
- Dynamically allows new players to join as current players finish their sessions
- Prevents server overload and maintains game performance
- Provides a smooth, organized player experience during peak hours

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define GAMERS_NUM 16

sem_t semaphore;

void* server(void* args){
    int index = *(int *)args;
    free(args);
    printf("BUSY:\tGamer[%d]\tis waiting....\n", index);
    sem_wait(&semaphore);
    printf("INSIDE:\tGamer[%d] is logged in.\n", index);
    sleep(1);
    printf("INSIDE:\tGamer[%d]\tis logged out.\n", index);
    sem_post(&semaphore);
}


int main(){

    pthread_t gamers[GAMERS_NUM];
    sem_init(&semaphore, 0, 12); // Letting only 12 gamers into the server (at a given time)

    for(int i = 0; i<16; i++){
        int* a = malloc(sizeof(int));
        *a = i;
        if(pthread_create(&gamers[i], NULL, &server, (void *)a)!=0){
            perror("Failed to create Thread");
        }
    }

    for(int i = 0; i<16; i++){
        if(pthread_join(gamers[i], NULL)!=0){
            perror("Failed to join Thread");
        }
    }

    sem_destroy(&semaphore);
    return 0;
}

/*

BUSY:	Gamer[0]	is waiting....
INSIDE:	Gamer[0] is logged in.
BUSY:	Gamer[1]	is waiting....
INSIDE:	Gamer[1] is logged in.
BUSY:	Gamer[2]	is waiting....
INSIDE:	Gamer[2] is logged in.
BUSY:	Gamer[3]	is waiting....
INSIDE:	Gamer[3] is logged in.
BUSY:	Gamer[4]	is waiting....
INSIDE:	Gamer[4] is logged in.
BUSY:	Gamer[5]	is waiting....
INSIDE:	Gamer[5] is logged in.
BUSY:	Gamer[6]	is waiting....
INSIDE:	Gamer[6] is logged in.
BUSY:	Gamer[7]	is waiting....
INSIDE:	Gamer[7] is logged in.
BUSY:	Gamer[8]	is waiting....
INSIDE:	Gamer[8] is logged in.
BUSY:	Gamer[9]	is waiting....
INSIDE:	Gamer[9] is logged in.
BUSY:	Gamer[10]	is waiting....
INSIDE:	Gamer[10] is logged in.
BUSY:	Gamer[11]	is waiting....
INSIDE:	Gamer[11] is logged in.
BUSY:	Gamer[12]	is waiting....
BUSY:	Gamer[13]	is waiting....
BUSY:	Gamer[14]	is waiting....
BUSY:	Gamer[15]	is waiting....
INSIDE:	Gamer[0]	is logged out.
INSIDE:	Gamer[1]	is logged out.
INSIDE:	Gamer[2]	is logged out.
INSIDE:	Gamer[12] is logged in.
INSIDE:	Gamer[13] is logged in.
INSIDE:	Gamer[4]	is logged out.
INSIDE:	Gamer[3]	is logged out.
INSIDE:	Gamer[14] is logged in.
INSIDE:	Gamer[6]	is logged out.
INSIDE:	Gamer[7]	is logged out.
INSIDE:	Gamer[5]	is logged out.
INSIDE:	Gamer[15] is logged in.
INSIDE:	Gamer[8]	is logged out.
INSIDE:	Gamer[9]	is logged out.
INSIDE:	Gamer[11]	is logged out.
INSIDE:	Gamer[10]	is logged out.
INSIDE:	Gamer[12]	is logged out.
INSIDE:	Gamer[13]	is logged out.
INSIDE:	Gamer[14]	is logged out.
INSIDE:	Gamer[15]	is logged out.


NOTE:
12 gamers wait and login instantly
4 gamers wait for some time so that some gamers logout
then eventually everyone login and at the end everyone logout

*/


