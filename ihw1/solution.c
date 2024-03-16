#include <stdio.h>
#include <ctype.h>
#include <string.h>

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
    char str[1000];
    printf("Введите ASCII-строку: ");
    fgets(str, sizeof(str), stdin);

    int count = 0;
    int identifiers[1000] = {0}; // Массив для хранения уникальных идентификаторов
    char *token = strtok(str, " ,.!?;:\n\t"); // Разделители

    while (token != NULL) {
        if (is_valid_identifier(token)) {
            int is_new = 1;
            for (int i = 0; i < count; i++) {
                if (strcmp(token, &str[identifiers[i]]) == 0) {
                    is_new = 0;
                    break;
                }
            }
            if (is_new) {
                identifiers[count++] = token - str;
            }
        }
        token = strtok(NULL, " ,.!?;:\n\t");
    }

    printf("Количество различных идентификаторов: %d\n", count);

    return 0;
}
