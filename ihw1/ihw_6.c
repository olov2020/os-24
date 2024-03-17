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
    int channel1[2];
    int channel2[2];
    pid_t pid1, pid2;

    if (pipe(channel1) == -1 || pipe(channel2) == -1) {
        perror("Ошибка при создании каналов");
        exit(EXIT_FAILURE);
    }

    printf("Введите путь к файлу ввода: ");
    fgets(input_filename, sizeof(input_filename), stdin);
    input_filename[strcspn(input_filename, "\n")] = '\0'; // Удаляем символ новой строки

    printf("Введите путь к файлу вывода: ");
    fgets(output_filename, sizeof(output_filename), stdin);
    output_filename[strcspn(output_filename, "\n")] = '\0'; // Удаляем символ новой строки

    pid1 = fork();

    if (pid1 < 0) {
        perror("Ошибка при создании процесса");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Код первого процесса (читает из файла и передает через неименованный канал)
        close(channel1[0]);  // Закрываем чтение канала
        int input_fd = open(input_filename, O_RDONLY);
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
            write(channel1[1], buffer, bytes_read);
        }
        close(input_fd);
        close(channel1[1]);
        exit(EXIT_SUCCESS);
    } else {
        pid2 = fork();

        if (pid2 < 0) {
            perror("Ошибка при создании процесса");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0) {
            // Код второго процесса (обработка данных)
            close(channel1[1]);  // Закрываем запись канала 1
            close(channel2[0]);  // Закрываем чтение канала 2
            dup2(channel1[0], STDIN_FILENO);
            dup2(channel2[1], STDOUT_FILENO);
            execl("./solution", "solution", NULL);
            perror("Ошибка при запуске второго процесса");
            exit(EXIT_FAILURE);
        } else {
            // Код третьего процесса (вывод в файл)
            close(channel1[0]);
            close(channel2[1]);
            int output_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            char buffer[BUFFER_SIZE];
            ssize_t bytes_read;
            while ((bytes_read = read(channel2[0], buffer, BUFFER_SIZE)) > 0) {
                write(output_fd, buffer, bytes_read);
            }
            close(output_fd);
            close(channel2[0]);
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}
