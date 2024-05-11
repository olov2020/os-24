#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: %s <server IP> <server port>n", argv[0]);
        return 1;
    }

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    char buffer[1024];

    // Создание сокета сервера
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        return 1;
    }

    // Настройка адреса и порта сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // Привязка сокета к адресу и порту
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        return 1;
    }

    // Прослушивание сокета
    if (listen(server_sock, 5) < 0) {
        perror("listen");
        return 1;
    }

    // Бесконечный цикл для обработки клиентов
    while (1) {
        // Принятие соединения от клиента
        client_addr_len = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }

        // Чтение сообщения от клиента
        ssize_t bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
        if (bytes_received < 0) {
            perror("recv");
            close(client_sock);
            continue;
        }

        // Проверка сообщения на завершение работы
        if (strcmp(buffer, "The End") == 0) {
            printf("Received "The End" from client %s:%dn", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            break;
        }

        // Пересылка сообщения клиенту №2
        // ... (код для подключения к клиенту №2 и отправки сообщения)

        // Закрытие сокета клиента
        close(client_sock);
    }

    // Закрытие сокета сервера
    close(server_sock);

    return 0;
}
