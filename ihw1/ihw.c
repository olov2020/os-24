#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main() {
    char input_filename[BUFFER_SIZE];
    char output_filename[BUFFER_SIZE];
    int fd1[2], fd2[2];
    pid_t pid1, pid2;

    printf("Введите путь к файлу ввода: ");
    fgets(input_filename, sizeof(input_filename), stdin);
    input_filename[strcspn(input_filename, "\n")] = '\0'; // Удаляем символ новой строки

    printf("Введите путь к файлу вывода: ");
    fgets(output_filename, sizeof(output_filename), stdin);
    output_filename[strcspn(output_filename, "\n")] = '\0'; // Удаляем символ новой строки

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
        perror("Ошибка при запуске первого процесса");
        exit(EXIT_FAILURE);
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
            execl("solution", "solution", NULL); // Запускаем программу для обработки данных

            perror("Ошибка при запуске второго процесса");
            exit(EXIT_FAILURE);
        } else {
            // Код третьего процесса (вывод в файл)
            close(fd1[0]); // Закрываем чтение
            close(fd1[1]);
            close(fd2[1]); // Закрываем запись
            int output_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (output_fd == -1) {
                perror("Ошибка открытия файла вывода");
                exit(EXIT_FAILURE);
            }
            dup2(fd2[0], STDIN_FILENO); // Перенаправляем ввод из канала
            dup2(output_fd, STDOUT_FILENO); // Перенаправляем вывод в файл
            execlp("cat", "cat", NULL); // Выводим в файл output_filename
            perror("Ошибка при запуске третьего процесса");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
