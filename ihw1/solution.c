#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

int main() {
    char input_line[BUFFER_SIZE];
    int num_identifiers = 0;

    while(fgets(input_line, sizeof(input_line), stdin) != NULL) {
        char* token = strtok(input_line, " \t\n\r"); // Разделители: пробел, табуляция, новая строка, возврат каретки
        while(token != NULL) {
            if(isalpha(token[0])) { // Проверяем, начинается ли идентификатор с буквы
                num_identifiers++;
            }
            token = strtok(NULL, " \t\n\r");
        }
    }

    printf("Количество идентификаторов: %d\n", num_identifiers);

    return 0;
}
