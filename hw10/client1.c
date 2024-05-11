#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *end_message = "The End";
    char message[1024];

    // Создание сокета клиента
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Настройка адреса сервера
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Преобразование IP-адреса в двоичный формат
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Установление соединения с сервером
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Цикл отправки сообщений
    while (1) {
        // Ввод сообщения
        printf("Enter message for Client 2: ");
        fgets(message, 1024, stdin);

        // Проверка завершения работы
        if (strcmp(message, end_message) == 0) {
            printf("End of communication\n");
            break;
        }

        // Отправка сообщения серверу
        send(sock, message, strlen(message), 0);
    }

    // Закрытие сокета
    close(sock);
    return 0;
}
