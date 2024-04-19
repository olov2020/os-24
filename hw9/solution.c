#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>

#define MESSAGE_COUNT 10

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    sem_t semaphore;
    if (sem_init(&semaphore, 1, 0) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        close(pipefd[1]); // Close write end
        int i;
        for (i = 0; i < MESSAGE_COUNT; i++) {
            sem_wait(&semaphore); // Wait for signal from parent
            char message[100];
            read(pipefd[0], message, sizeof(message));
            printf("Child received: %s\n", message);
            sem_post(&semaphore); // Signal parent
        }
        close(pipefd[0]); // Close read end
        sem_destroy(&semaphore);
        exit(EXIT_SUCCESS);
    } else { // Parent process
        close(pipefd[0]); // Close read end
        int i;
        for (i = 0; i < MESSAGE_COUNT; i++) {
            char message[100];
            sprintf(message, "Message %d from parent", i+1);
            write(pipefd[1], message, sizeof(message));
            sem_post(&semaphore); // Signal child
            sem_wait(&semaphore); // Wait for signal from child
        }
        close(pipefd[1]); // Close write end
        sem_destroy(&semaphore);
        wait(NULL); // Wait for child to finish
        exit(EXIT_SUCCESS);
    }
}
