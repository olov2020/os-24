#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 5000

int main() {
    char input_filename[BUFFER_SIZE];
    char output_filename[BUFFER_SIZE];
    int fd1[2], fd2[2];
    pid_t pid1, pid2;
    char buffer[BUFFER_SIZE];

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
        // Процесс чтения из файла и передачи через канал
        close(fd1[0]); // Закрываем чтение
        int input_fd = open(input_filename, O_RDONLY);
        if (input_fd == -1) {
            perror("Ошибка открытия файла для чтения");
            exit(EXIT_FAILURE);
        }
        while (read(input_fd, buffer, BUFFER_SIZE) > 0) {
            write(fd1[1], buffer, BUFFER_SIZE);
        }
        close(fd1[1]); // Закрываем запись
        exit(EXIT_SUCCESS);
    } else {
        pid2 = fork();

        if (pid2 < 0) {
            perror("Ошибка при создании процесса");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0) {
            // Процесс обработки данных
            close(fd1[1]); // Закрываем запись
            close(fd2[0]); // Закрываем чтение

            while (read(fd1[0], buffer, BUFFER_SIZE) > 0) {
                // Обработка данных (можно добавить соответствующую логику)
                // Например, преобразование текста
                // Пример обработки: просто переворачиваем текст
                for (int i = 0, j = strlen(buffer)-1; i < j; i++, j--) {
                    char temp = buffer[i];
                    buffer[i] = buffer[j];
                    buffer[j] = temp;
                }
                
                write(fd2[1], buffer, BUFFER_SIZE); // Передаем данные обратно
            }
            close(fd1[0]); // Закрываем чтение
            close(fd2[1]); // Закрываем запись
            exit(EXIT_SUCCESS);
        } else {
            // Процесс вывода в файл
            close(fd1[0]); // Закрываем чтение
            close(fd2[1]); // Закрываем запись
            int output_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (output_fd == -1) {
                perror("Ошибка открытия файла вывода");
                exit(EXIT_FAILURE);
            }
            while (read(fd2[0], buffer, BUFFER_SIZE) > 0) {
                write(output_fd, buffer, BUFFER_SIZE);
            }
            close(fd2[0]); // Закрываем чтение
            close(output_fd); // Закрываем файл вывода
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}
