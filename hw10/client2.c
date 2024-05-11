#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <serverip> <serverport>\n", argv[0]);
        exit(1);
    }

    // Создать сокет клиента
    int clientsock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsock == -1) {
        perror("socket");
        exit(1);
    }

    // Настроить адрес и порт сервера
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));

    // Установить соединение с сервером
    if (connect(clientsock, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
        perror("connect");
        exit(1);
    }

    // Читать сообщения от сервера и выводить их на консоль
    char buffer[1024];
    while (1) {
        // Читать сообщение от сервера
        int n = recv(clientsock, buffer, sizeof(buffer), 0);
        if (n == -1) {
            perror("recv");
            exit(1);
        }

        if (strcmp(buffer, "The End") == 0) {
            // Получили сообщение "The End" - завершаем работу
            close(clientsock);
            exit(0);
        }

        // Вывести сообщение на консоль
        printf("Получено сообщение: %s", buffer);
    }

    return 0;
}
