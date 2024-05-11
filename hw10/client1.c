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

    // Создать сокет клиента
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock == -1) {
        perror("socket");
        exit(1);
    }

    // Настроить адрес и порт сервера
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // Установить соединение с сервером
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(1);
    }

    // В цикле читать сообщения с консоли и отправлять их серверу
    char buffer[1024];
    while (1) {
        // Прочитать сообщение с консоли
        printf("Введите сообщение: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Отправить сообщение серверу
        if (send(client_sock, buffer, strlen(buffer), 0) == -1) {
            perror("send");
            exit(1);
        }

        if (strcmp(buffer, "The End\n") == 0) {
            // Отправили сообщение "The End" - завершаем работу
            close(client_sock);
            exit(0);
        }
    }

    return 0;
}
