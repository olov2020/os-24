#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip_address> <server_port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Создание сокета клиента
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    // Настройка адреса сервера
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));

    // Соединение с сервером
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("connect");
        return EXIT_FAILURE;
    }

    // Ввод сообщений
    char buffer[1024];
    while (1) {
        printf("Enter message (The End to quit): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Отправка сообщения серверу
        if (strcmp(buffer, "The End\n") == 0) {
            break;
        }
        send(client_socket, buffer, strlen(buffer), 0);
    }

    // Закрываем сокет клиента
    close(client_socket);
    return EXITSUCCESS;
}
