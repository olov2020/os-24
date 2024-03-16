#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main() {
    char input_filename[BUFFER_SIZE];
    char output_filename[BUFFER_SIZE];
    int fd1[2], fd2[2];
    pid_t pid1, pid2;

    printf("Введите путь к файлу ввода: ");
    scanf("%s", input_filename);

    printf("Введите путь к файлу вывода: ");
    scanf("%s", output_filename);

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("Ошибка создания канала");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();

    if (pid1 < 0) {
        perror("Ошибка при создании процесса");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Код первого процесса (читает из файла и передает через канал)
        close(fd1[0]); // Закрываем чтение
        dup2(fd1[1], STDOUT_FILENO); // Перенаправляем вывод в канал
        execlp("cat", "cat", input_filename, NULL); // Читаем из файла input_filename
        close(fd1[1]);
    } else {
        pid2 = fork();

        if (pid2 < 0) {
            perror("Ошибка при создании процесса");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0) {
            // Код второго процесса (обработка данных)
            close(fd1[1]); // Закрываем запись
            close(fd2[0]); // Закрываем чтение
            dup2(fd1[0], STDIN_FILENO); // Перенаправляем ввод из канала
            dup2(fd2[1], STDOUT_FILENO); // Перенаправляем вывод в канал
            execlp("./process_data", "./process_data", NULL); // Запускаем программу для обработки данных
            close(fd1[0]);
            close(fd2[1]);
        } else {
            // Код третьего процесса (вывод в файл)
            close(fd1[0]); // Закрываем чтение
            close(fd1[1]);
            close(fd2[1]); // Закрываем запись
            dup2(fd2[0], STDIN_FILENO); // Перенаправляем ввод из канала
            execlp("cat", "cat", output_filename, NULL); // Выводим в файл output_filename
            close(fd2[0]);
        }
    }

    return 0;
}
