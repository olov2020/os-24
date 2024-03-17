#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 5000

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

int main() {
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

    return 0;
}
