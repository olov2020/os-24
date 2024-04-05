#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("server-client", 65);
    int shmid = shmget(key, SHM_SIZE, 0666);
    char *data = (char*)shmat(shmid, (void*)0, 0);

    while(1) {
        int random_num = rand() % 100; // Generate random number
        sprintf(data, "%d", random_num);
        sleep(1); // Delay before sending next number
    }

    shmdt(data);

    return 0;
}
