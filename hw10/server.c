#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

int main(int argc, char** argv) {
    int server_sockfd, client1_sockfd, client2_sockfd;
    struct sockaddr_in server_addr, client1_addr, client2_addr;
    socklen_t client1_addr_len, client2_addr_len;
    char buffer[1024];

    // Создать серверный сокет
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // Настроить серверный сокет
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Привязать сокет к адресу
    if (bind(server_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    // Начать прослушивание
    if (listen(server_sockfd, 5) < 0) {
        perror("listen");
        exit(1);
    }

    // Принять подключение от клиента №1
    client1_addr_len = sizeof(client1_addr);
    client1_sockfd = accept(server_sockfd, (struct sockaddr*)&client1_addr, &client1_addr_len);
    if (client1_sockfd < 0) {
        perror("accept");
        exit(1);
    }

    // Принять подключение от клиента №2
    client2_addr_len = sizeof(client2_addr);
    client2_sockfd = accept(server_sockfd, (struct sockaddr*)&client2_addr, &client2_addr_len);
    if (client2_sockfd < 0) {
        perror("accept");
        exit(1);
    }

    // Обрабатывать сообщения до получения сообщения "The End"
    while (1) {
        // Получить сообщение от клиента №1
        memset(buffer, 0, sizeof(buffer));
        recv(client1_sockfd, buffer, sizeof(buffer), 0);

        // Проверить, является ли сообщение сообщением завершения
        if (strcmp(buffer, "The End") == 0) {
            break;
        }

        // Перенаправить сообщение клиенту №2
        send(client2_sockfd, buffer, strlen(buffer), 0);
    }

    // Закрыть сокеты
    close(client1_sockfd);
    close(client2_sockfd);
    close(server_sockfd);

    return 0;
}
