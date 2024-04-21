#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int pot_of_missionaries;

void eat_missionary(int id) {
    printf("Дикарь %d ест кусок тушеного миссионера\n", id);
    pot_of_missionaries--;
}

void cook_missionaries() {
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

    // Создание процесса для повара
    if ((pid = fork()) == 0) {
        while (1) {
            // Повар спит, пока не проснется первый дикарь
            sleep(5);
            cook_missionaries();
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
                    eat_missionary(i);
                    sleep(1); // Дикарь ест кусок тушеного миссионера
                } else {
                    printf("Дикарь %d будит повара\n", i);
                    // Будим повара и ждем, пока он приготовит обед
                    kill(getppid(), SIGUSR1);
                    sleep(5);
                }
            }
        } else if (pid < 0) {
            perror("Ошибка при создании процесса для дикаря");
            exit(EXIT_FAILURE);
        }
    }

    // Обработка сигнала от дикарей для повара
    signal(SIGUSR1, cook_missionaries);

    // Ожидание завершения всех процессов
    for (i = 0; i < N + 1; i++) {
        wait(NULL);
    }

    return 0;
}