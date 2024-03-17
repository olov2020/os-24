#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>

#define BUFFER_SIZE 5000


void read_from_file(int fd, char* buffer, size_t size) {
    ssize_t bytes_read = read(fd, buffer, size);
    if (bytes_read == -1) {
        perror("Ошибка при чтении файла");
        exit(EXIT_FAILURE);
    }
}

void write_to_file(int fd, char* buffer, size_t size) {
    ssize_t bytes_written = write(fd, buffer, size);
    if (bytes_written == -1) {
        perror("Ошибка при записи в файл");
        exit(EXIT_FAILURE);
    }
}

int is_valid_identifier(char *str) {
    if (!isalpha(str[0])) {
        return 0; // Проверка на начало с буквы
    }
    for (int i = 1; i < strlen(str); i++) {
        if (!isalnum(str[i])) {
            return 0; // Проверка на буквы и цифры
        }
    }
    return 1;
}

void count(fd2) {
    char input_line[BUFFER_SIZE];
    fgets(input_line, sizeof(input_line), stdin);

    int count = 0;
    int identifiers[1000] = {0}; // Массив для хранения уникальных идентификаторов
    char *token = strtok(input_line, " ,.!?;:-=\n\t"); // Разделители

    while (token != NULL) {
        if (is_valid_identifier(token)) {
            int is_new = 1;
            for (int i = 0; i < count; i++) {
                if (strcmp(token, &input_line[identifiers[i]]) == 0) {
                    is_new = 0;
                    break;
                }
            }
            if (is_new) {
                identifiers[count++] = token - input_line;
            }
        }
        token = strtok(NULL, " ,.!?;:\n\t");
    }

    printf("Количество различных идентификаторов: %d\n", count);
    write_to_file(fd2[1], &count, sizeof(int));
}

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
        int input_fd = open(input_filename, O_RDONLY);
        if (input_fd == -1) {
            perror("Ошибка открытия файла ввода");
            exit(EXIT_FAILURE);
        }
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
            write_to_file(fd1[1], buffer, bytes_read);
        }
        close(input_fd);
        close(fd1[1]);
        exit(EXIT_SUCCESS);
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
            count(fd2);
            close(fd1[0]);
            close(fd2[1]);
            exit(EXIT_SUCCESS);
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
            int num_identifiers;
            read_from_file(fd2[0], &num_identifiers, sizeof(int));
            char num_identifiers_str[BUFFER_SIZE];
            sprintf(num_identifiers_str, "%d\n", num_identifiers);
            write_to_file(output_fd, num_identifiers_str, strlen(num_identifiers_str));
            close(fd2[0]);
            close(output_fd);
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}
