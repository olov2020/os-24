#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 5 // Количество дикарей
#define M 10 // Вместимость горшка

int pot = 0;

void cook() {
    printf("Повар начал готовить обед.\n");
    usleep(3000000); // Имитация приготовления еды
    pot = M;
    printf("Обед готов!\n");
}

void eat(int id) {
    printf("Дикарь %d начал обедать.\n", id);
    while (pot == 0) {
        printf("Дикарь %d будил повара.\n", id);
        cook();
    }
    pot--;
    printf("Дикарь %d закончил обедать.\n", id);
}

int main() {
    pid_t pid;

    for (int i = 1; i <= N; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            eat(i);
            exit(0);
        }
    }

    while (wait(NULL) > 0); // Ждем завершения всех процессов дикарей

    return 0;
}
