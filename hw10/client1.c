#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: %s <server IP> <server port>n", argv[0]);
        return 1;
    }

    int client_sock;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Создание сокета клиента
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        perror("socket");
        return 1;
    }

    // Настройка адреса и порта сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // Подключение к серверу
    if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;
    }

    // Ввод сообщений с клавиатуры
    while (1) {
        printf("Enter a message (or "The End" to quit): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Проверка сообщения на завершение работы
        if (strcmp(buffer, "The Endn") == 0) {
            break;
        }

        // Отправка сообщения серверу
        if (send(client_sock, buffer, strlen(buffer), 0) < 0) {
            perror("send");
            break;
        }
    }

    // Закрытие сокета клиента
    close(client_sock);

    return 0;
}
