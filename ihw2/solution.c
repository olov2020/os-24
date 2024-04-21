#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define M 10 // Количество кусков тушеного миссионера
#define N 5 // Количество дикарей

int main() {
    int pot = M; // Горшок с кусками тушеного миссионера
    int pid;

    for (int i = 0; i < N; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            // Дикарь
            while (1) {
                if (pot == 0) {
                    printf("Дикарь %d будит повара\n", i + 1);
                    pot = M;
                } else {
                    printf("Дикарь %d ест кусок тушеного миссионера. Осталось: %d\n", i + 1, pot);
                    pot--;
                }
            }
        }
    }

    // Повар
    while (1) {
        sleep(1); // Повар засыпает
        printf("Повар заполняет горшок\n");
        pot = M;
    }

    for (int i = 0; i < N; i++) {
        wait(NULL); // Ждем завершения всех дикарей
    }

    return 0;
}
