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
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    char *data = (char*)shmat(shmid, (void*)0, 0);

    while(1) {
        if(data[0] != '\0') {
            printf("Received number from client: %s\n", data);
            data[0] = '\0';
        }
        usleep(1000); // Delay to prevent high CPU usage
    }

    shmdt(data);
    
    // Remove shared memory segment
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
