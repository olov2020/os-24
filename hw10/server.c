#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(1);
    }

    // Создать сокет сервера
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("socket");
        exit(1);
    }

    // Настроить адрес и порт сервера
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // Привязать сокет к адресу и порту
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    // Слушать подключения клиентов
    if (listen(server_sock, 2) == -1) {
        perror("listen");
        exit(1);
    }

    // Принять подключения от клиентов
    int client1_sock, client2_sock;
    struct sockaddr_in client1_addr, client2_addr;
    socklen_t client1_addr_len, client2_addr_len;

    client1_sock = accept(server_sock, (struct sockaddr *)&client1_addr, &client1_addr_len);
    if (client1_sock == -1) {
        perror("accept");
        exit(1);
    }

    client2_sock = accept(server_sock, (struct sockaddr *)&client2_addr, &client2_addr_len);
    if (client2_sock == -1) {
        perror("accept");
        exit(1);
    }

    // Читать сообщения от клиента №1 и перенаправлять их клиенту №2
    char buffer[1024];
    while (1) {
        // Читать сообщение от клиента №1
        int n = recv(client1_sock, buffer, sizeof(buffer), 0);
        if (n == -1) {
            perror("recv");
            exit(1);
        }

        if (strcmp(buffer, "The End") == 0) {
            // Получили сообщение "The End" - завершаем работу
            close(client1_sock);
            close(client2_sock);
            close(server_sock);
            exit(0);
        }

        // Перенаправить сообщение клиенту №2
        if (send(client2_sock, buffer, n, 0) == -1) {
            perror("send");
            exit(1);
        }
    }

    return 0;
}
