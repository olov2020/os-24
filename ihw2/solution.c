#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 5 // Количество дикарей
#define M 10 // Вместимость горшка

int pot = 0; // Количество кусков в горшке

void cook() {
    while(1) {
        if (pot == 0) {
            printf("Повар просыпается и готовит еду...\n");
            pot = M;
            printf("Повар наполнил горшок кусками тушеного миссионера.\n");
        }
        sleep(2); // Повар засыпает на некоторое время
    }
}

void savage(int id) {
    while(1) {
        if (pot > 0) {
            printf("Дикарь %d ест кусок тушеного миссионера. Осталось в горшке: %d\n", id, --pot);
            sleep(1); // Дикарь ест кусок
        } else {
            printf("Дикарь %d будит повара.\n", id);
            // Сигнализируем повару о том, что горшок пуст
            // На практике можно использовать механизм семафоров или мьютексов для синхронизации процессов
        }
    }
}

int main() {
    pid_t pid;
    int i;

    // Создаем процесс повара
    pid = fork();
    if (pid == 0) {
        cook(); // Внутри процесса-повара вызываем функцию, которая будет готовить еду
    } else if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // Создаем процессы дикарей
    for (i = 1; i <= N; i++) {
        pid = fork();
        if (pid == 0) {
            savage(i); // Внутри процессов-дикарей вызываем функцию, которая будет есть
            break; // Прерываем цикл в дочернем процессе
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // Ждем завершения всех процессов
    while (wait(NULL) > 0);

    return 0;
}
