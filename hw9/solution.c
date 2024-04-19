#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define NUM_MESSAGES 10

int sem_id;

void parent_process(int pipe_fd[2]) {
    int i;
    char message[256];
    
    for (i = 0; i < NUM_MESSAGES; ++i) {
        printf("Parent process sending message %d\n", i);
        sprintf(message, "Message from parent %d", i);
        write(pipe_fd[1], message, sizeof(message));
        sleep(1);
        read(pipe_fd[0], message, sizeof(message));
        printf("Parent process received: %s\n", message);
    }

    // Отправка сигнала завершения дочернему процессу
    kill(getpid(), SIGINT);
}

void child_process(int pipe_fd[2]) {
    int i;
    char message[256];
    
    for (i = 0; i < NUM_MESSAGES; ++i) {
        read(pipe_fd[0], message, sizeof(message));
        printf("Child process received: %s\n", message);
        sleep(1);
        printf("Child process sending message %d\n", i);
        sprintf(message, "Message from child %d", i);
        write(pipe_fd[1], message, sizeof(message));
    }
}

void signal_handler(int sig) {
    printf("Signal received, exiting.\n");
    // Удаление семафора
    semctl(sem_id, 0, IPC_RMID);
    exit(0);
}

int main() {
    int pipe_fd[2];

    // Создание канала
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    // Установка обработчика сигнала
    signal(SIGINT, signal_handler);

    // Создание дочернего процесса
    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        return 1;
    } else if (pid > 0) {
        // Родительский процесс
        close(pipe_fd[0]);  // Закрыть чтение в родительском процессе
        parent_process(pipe_fd);
    } else {
        // Дочерний процесс
        close(pipe_fd[1]);  // Закрыть запись в дочернем процессе
        child_process(pipe_fd);
    }

    return 0;
}
