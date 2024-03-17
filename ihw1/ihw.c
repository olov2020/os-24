#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define BUFFER_SIZE 5000

int main() {
    char input_filename[BUFFER_SIZE];
    char output_filename[BUFFER_SIZE];
    char fifo1[] = "/tmp/fifo1";
    char fifo2[] = "/tmp/fifo2";
    pid_t pid1, pid2;

    printf("Введите путь к файлу ввода: ");
    fgets(input_filename, sizeof(input_filename), stdin);
    input_filename[strcspn(input_filename, "\n")] = '\0'; // Удаляем символ новой строки

    printf("Введите путь к файлу вывода: ");
    fgets(output_filename, sizeof(output_filename), stdin);
    output_filename[strcspn(output_filename, "\n")] = '\0'; // Удаляем символ новой строки

    if (mkfifo(fifo1, 0666) == -1 || mkfifo(fifo2, 0666) == -1) {
        perror("Ошибка создания именованных каналов");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();

    if (pid1 < 0) {
        perror("Ошибка при создании процесса");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Код первого процесса (читает из файла и передает через канал)
        int fd1 = open(fifo1, O_WRONLY);
        dup2(fd1, STDOUT_FILENO); // Перенаправляем вывод в канал
        execlp("cat", "cat", input_filename, NULL);
        perror("Ошибка при запуске первого процесса");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL); // Ждем завершения первого процесса

        pid2 = fork();

        if (pid2 < 0) {
            perror("Ошибка при создании процесса");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0) {
            // Код второго процесса (обработка данных)
            int fd1 = open(fifo1, O_RDONLY);
            int fd2 = open(fifo2, O_WRONLY);
            dup2(fd1, STDIN_FILENO); // Перенаправляем ввод из канала
            dup2(fd2, STDOUT_FILENO); // Перенаправляем вывод в канал
            execl("./solution", "solution", NULL); // Запускаем программу для обработки данных
            perror("Ошибка при запуске второго процесса");
            exit(EXIT_FAILURE);
        } else {
            wait(NULL); // Ждем завершения второго процесса

            // Код третьего процесса (вывод в файл)
            int fd2 = open(fifo2, O_RDONLY);
            int output_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (output_fd == -1) {
                perror("Ошибка открытия файла вывода");
                exit(EXIT_FAILURE);
            }
            dup2(fd2, STDIN_FILENO); // Перенаправляем ввод из канала
            dup2(output_fd, STDOUT_FILENO); // Перенаправляем вывод в файл
            execlp("cat", "cat", NULL); // Выводим в файл output_filename
            perror("Ошибка при запуске третьего процесса");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
