#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 5000

int main() {
    char input_filename[BUFFER_SIZE];
    char output_filename[BUFFER_SIZE];
    char fifo1[] = "fifo1";
    char fifo2[] = "fifo2";
    pid_t pid1, pid2;

    printf("Введите путь к файлу ввода: ");
    fgets(input_filename, sizeof(input_filename), stdin);
    input_filename[strcspn(input_filename, "\n")] = '\0'; // Удаляем символ новой строки

    printf("Введите путь к файлу вывода: ");
    fgets(output_filename, sizeof(output_filename), stdin);
    output_filename[strcspn(output_filename, "\n")] = '\0'; // Удаляем символ новой строки

    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);

    pid1 = fork();

    if (pid1 < 0) {
        perror("Ошибка при создании процесса");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Код первого процесса (читает из файла и передает через именованный канал)
        int fd1 = open(fifo1, O_WRONLY);
        if (fd1 == -1) {
            perror("Ошибка открытия именованного канала 1");
            exit(EXIT_FAILURE);
        }

        int input_fd = open(input_filename, O_RDONLY);
        if (input_fd == -1) {
            perror("Ошибка открытия файла ввода");
            exit(EXIT_FAILURE);
        }

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
            write(fd1, buffer, bytes_read);
        }

        close(input_fd);
        close(fd1);
        exit(EXIT_SUCCESS);
    } else {
        pid2 = fork();

        if (pid2 < 0) {
            perror("Ошибка при создании процесса");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0) {
            // Код второго процесса (обработка данных)
            int fd1 = open(fifo1, O_RDONLY);
            int fd2 = open(fifo2, O_WRONLY);

            dup2(fd1, STDIN_FILENO); // Перенаправляем ввод из именованного канала 1
            dup2(fd2, STDOUT_FILENO); // Перенаправляем вывод в именованный канал 2
            execl("solution", "solution", NULL); // Запускаем программу для обработки данных

            perror("Ошибка при запуске второго процесса");
            exit(EXIT_FAILURE);
        } else {
            // Код третьего процесса (вывод из именованного канала в файл)
            int fd2 = open(fifo2, O_RDONLY);
            int output_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);

            char buffer[BUFFER_SIZE];
            ssize_t bytes_read;
            while ((bytes_read = read(fd2, buffer, BUFFER_SIZE)) > 0) {
                write(output_fd, buffer, bytes_read);
            }

            close(fd2);
            close(output_fd);
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}
