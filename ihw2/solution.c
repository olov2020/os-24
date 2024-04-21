#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Number of savages
#define NUM_SAVAGES 5

// Capacity of the pot
#define POT_CAPACITY 10

// Semaphore to protect the pot
sem_t pot_mutex;

// Semaphore to signal the cook that the pot is empty
sem_t pot_empty;

// Semaphore to signal the savages that the pot is full
sem_t pot_full;

// Function for the cook thread
void *cook(void *arg) {
    while (1) {
        // Wait for the pot to be empty
        sem_wait(&pot_empty);

        // Fill the pot
        printf("Cook: Filling the potn");
        sleep(1);

        // Signal the savages that the pot is full
        sem_post(&pot_full);
    }

    return NULL;
}

// Function for the savage thread
void *savage(void *arg) {
    int savage_id = (int) arg;

    while (1) {
        // Wait for the pot to be full
        sem_wait(&pot_full);

        // Enter the critical section
        sem_wait(&pot_mutex);

        // Check if the pot is empty
        if (POT_CAPACITY == 0) {
            // Wake up the cook
            sem_post(&pot_empty);

            // Wait for the pot to be full again
            sem_wait(&pot_full);
        }

        // Eat from the pot
        POT_CAPACITY--;
        printf("Savage %d: Eatingn", savage_id);
        sleep(1);

        // Leave the critical section
        sem_post(&pot_mutex);
    }

    return NULL;
}

int main() {
    // Initialize the semaphores
    sem_init(&pot_mutex, 0, 1);
    sem_init(&pot_empty, 0, 0);
    sem_init(&pot_full, 0, 0);

    // Create the cook thread
    pthread_t cook_thread;
    pthread_create(&cook_thread, NULL, cook, NULL);

    // Create the savage threads
    pthread_t savage_threads[NUM_SAVAGES];
    for (int i = 0; i < NUM_SAVAGES; i++) {
        pthread_create(&savage_threads[i], NULL, savage, (void *) i);
    }

    // Join the threads
    pthread_join(cook_thread, NULL);
    for (int i = 0; i < NUM_SAVAGES; i++) {
        pthread_join(savage_threads[i], NULL);
    }

    // Destroy the semaphores
    sem_destroy(&pot_mutex);
    sem_destroy(&pot_empty);
    sem_destroy(&pot_full);

    return 0;
}
