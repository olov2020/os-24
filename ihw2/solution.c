#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>

sem_t pot_sem; // Semaphore for access to the pot
sem_t cook_sem; // Semaphore for controlling the cook
int remaining_pieces = 0; // Number of remaining pieces of stewed missionary

void cook(int M) {
    printf("Cook starts preparing lunch.\n");
    remaining_pieces = M;
    printf("Cook prepared %d pieces of stewed missionary.\n", remaining_pieces);
    sem_post(&pot_sem); // Release the semaphore so the tribesmen can start eating
}

void eat(int id) {
    printf("Tribesman %d is hungry and wants to have lunch.\n", id);
    sem_wait(&pot_sem); // Wait for access to the pot
    if (remaining_pieces > 0) {
        printf("Tribesman %d took a piece of stewed missionary from the pot.\n", id);
        remaining_pieces--;
    } else {
        printf("The pot is empty, tribesman %d wakes up the cook.\n", id);
        sem_post(&cook_sem); // Tribesman wakes up the cook
        sem_wait(&pot_sem); // Wait until the cook fills the pot
        printf("Tribesman %d took a piece of stewed missionary from the pot.\n", id);
        remaining_pieces--;
    }
    printf("Tribesman %d had lunch.\n", id);
}

void cook_process(int M) {
    while (1) {
        sem_wait(&cook_sem); // Wait until a tribesman wakes up the cook
        cook(M);
    }
}

void eat_process(int id, int N) {
    while (1) {
        eat(id);
        usleep(rand() % 1000000); // Tribesman will be hungry for a random time before the next meal
    }
}

int main() {
    int N, M;

    printf("Enter the number of tribesmen (N): ");
    if (scanf("%d", &N) != 1 || N <= 0) {
        printf("Invalid input for the number of tribesmen.\n");
        return 1;
    }

    printf("Enter the number of pieces of stewed missionary (M): ");
    if (scanf("%d", &M) != 1 || M <= 0) {
        printf("Invalid input for the number of pieces.\n");
        return 1;
    }

    sem_init(&pot_sem, 0, 1); // Semaphore initialization
    sem_init(&cook_sem, 0, 0); // Initialize cook semaphore

    pid_t pid;
    for (int i = 0; i < N; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) { // Child process
            eat_process(i+1, N);
            exit(0);
        }
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) { // Child process
        cook_process(M);
        exit(0);
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < N + 1; i++) {
        wait(NULL);
    }

    sem_destroy(&pot_sem); // Semaphore destruction
    sem_destroy(&cook_sem);

    return 0;
}
