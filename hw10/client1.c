#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char** argv) {
    int client_sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Задать IP-адрес и порт сервера
    char* server_ip = argv[1];
    int server_port = atoi(argv[2]);

    // Создать клиентский сокет
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // Настроить клиентский сокет
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    // Подключиться к серверу
    if (connect(client_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    // Отправлять сообщения до сообщения "The End"
    while (1) {
        // Получить сообщение из стандартного ввода
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);

        // Проверить, является ли сообщение сообщением завершения
        if (strcmp(buffer, "The End") == 0) {
            break;
        }

        // Отправить сообщение серверу
        send(client_sockfd, buffer, strlen(buffer), 0);
    }

    // Закрыть сокет
    close(client_sockfd);

    return 0;
}
