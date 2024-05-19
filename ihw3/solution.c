#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    int M, N;
    printf("Введите количество кусков тушеного миссионера (M): ");
    scanf("%d", &M);
    printf("Введите количество дикарей (N): ");
    scanf("%d", &N);

    // Создание разделяемой памяти для горшка с тушеным миссионером
    int shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget");
        exit(1);
    }

    // Присоединение разделяемой памяти
    int *pot = (int *)shmat(shm_id, NULL, 0);
    if (*pot == -1) {
        perror("shmat");
        exit(1);
    }
    *pot = M;

    int pid;

    for (int i = 0; i < N; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            while (1) {
                if (*pot == 0) {
                    printf("Дикарь %d будит повара\n", i + 1);
                    *pot = M;
                } else {
                    printf("Дикарь %d ест кусок тушеного миссионера. Осталось: %d\n", i + 1, *pot);
                    (*pot)--;
                }
                sleep(1);
            }
        }
    }

    // Повар
    while (1) {
        sleep(1);
        printf("Повар заполняет горшок\n");
        *pot = M;
    }

    // Отсоединение разделяемой памяти
    shmdt(pot);
    // Удаление разделяемой памяти
    shmctl(shm_id, IPC_RMID, NULL);

    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    return 0;
}
