#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int pot_of_missionaries;

void eat_missionary(int id, int M) {
    printf("Дикарь %d ест кусок тушеного миссионера\n", id);
    pot_of_missionaries--;
}

void cook_missionaries(int M) {
    printf("Повар готовит обед\n");
    pot_of_missionaries = M;
}

int main() {
    pid_t pid;
    int i, N, M;

    // Read N and M from console
    printf("Введите количество дикарей (N): ");
    scanf("%d", &N);
    printf("Введите вместимость горшка (M): ");
    scanf("%d", &M);
    pot_of_missionaries = M;

    // Set up signal handler
    signal(SIGUSR1, cook_missionaries);

    // Создание процесса для повара
    if ((pid = fork()) == 0) {
        while (1) {
            // Повар спит, пока не проснется первый дикарь
            pause();
            cook_missionaries(M);
        }
    } else if (pid < 0) {
        perror("Ошибка при создании процесса для повара");
        exit(EXIT_FAILURE);
    }

    // Создание процессов для дикарей
    for (i = 1; i <= N; i++) {
        if ((pid = fork()) == 0) {
            while (1) {
                if (pot_of_missionaries > 0) {
                    eat_missionary(i, M);
                    sleep(1); // Дикарь ест кусок тушеного миссионера
                } else {
                    printf("Дикарь %d будит повара\n", i);
                    // Будим повара и ждем, пока он приготовит обед
                    kill(getppid(), SIGUSR1);
                    pause();
                }
            }
        } else if (pid < 0) {
            perror("Ошибка при создании процесса для дикаря");
            exit(EXIT_FAILURE);
        }
    }

    // Ожидание завершения всех процессов
    for (i = 0; i < N + 1; i++) {
        wait(NULL);
    }

    return 0;
}
