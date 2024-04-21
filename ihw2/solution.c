#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int M, N;
    printf("Введите количество кусков тушеного миссионера (M): ");
    scanf("%d", &M);
    printf("Введите количество дикарей (N): ");
    scanf("%d", &N);

    int pot = M; // Горшок с кусками тушеного миссионера
    int pid;

    for (int i = 0; i < N; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            while (1) {
                if (pot == 0) {
                    printf("Дикарь %d будит повара\n", i + 1);
                    pot = M;
                } else {
                    printf("Дикарь %d ест кусок тушеного миссионера. Осталось: %d\n", i + 1, pot);
                    pot--;
                }
                sleep(1);
            }
        }
    }

    // Повар
    while (1) {
        sleep(1);
        printf("Повар заполняет горшок\n");
        pot = M;
    }

    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    return 0;
}
