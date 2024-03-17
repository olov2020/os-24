#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 5000
#define FIFO_FILE "myfifo"

int main() {
    char input_filename[BUFFER_SIZE];
    char output_filename[BUFFER_SIZE];
    pid_t pid1, pid2;

    printf("Введите путь к файлу ввода: ");
    fgets(input_filename, sizeof(input_filename), stdin);
    input_filename[strcspn(input_filename, "")] = ''; // Удаляем символ новой строки

    printf("Введите путь к файлу вывода: ");
    fgets(output_filename, sizeof(output_filename), stdin);
    output_filename[strcspn(output_filename, "")] = ''; // Удаляем символ новой строки

    mkfifo(FIFO_FILE, 0666);

    pid1 = fork();

    if (pid1 < 0) {
        perror("Ошибка при создании процесса");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Код первого процесса (читает из файла и пишет в именованный канал)
        int fd = open(FIFO_FILE, O_WRONLY);
        execlp("cat", "cat", input_filename, NULL); // Читаем из файла input_filename и пишем в именованный канал
        perror("Ошибка при запуске первого процесса");
        exit(EXIT_FAILURE);
     else {
        pid2 = fork();

        if (pid2 < 0) {
            perror("Ошибка при создании процесса");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0) {
            // Код второго процесса (читает из именованного канала и обрабатывает данные)
            int fd = open(FIFO_FILE, O_RDONLY);
            dup2(fd, STDIN_FILENO); // Перенаправляем ввод из именованного канала
            int output_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(output_fd, STDOUT_FILENO); // Перенаправляем вывод в файл
            execl("solution", "solution", NULL); // Запускаем программу для обработки данных

            perror("Ошибка при запуске второго процесса");
            exit(EXIT_FAILURE);
         else {
            wait(NULL);
            remove(FIFO_FILE); // Удаляем именованный канал
        }
    }
     }
    }
    return 0;
}
