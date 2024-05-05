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
    clientsockfd = socket(AFINET, SOCKSTREAM, 0);
    if (clientsockfd < 0) {
        perror("socket");
        exit(1);
    }

    // Настроить клиентский сокет
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sinfamily = AFINET;
    serveraddr.sinaddr.saddr = inetaddr(serverip);
    serveraddr.sinport = htons(serverport);

    // Подключиться к серверу
    if (connect(clientsockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("connect");
        exit(1);
    }

    // Получать сообщения до сообщения "The End"
    while (1) {
        // Получить сообщение от сервера
        memset(buffer, 0, sizeof(buffer));
        recv(clientsockfd, buffer, sizeof(buffer), 0);

        // Проверить, является ли сообщение сообщением завершения
        if (strcmp(buffer, "The End") == 0) {
            break;
        }

        // Вывести сообщение
        printf("Получено сообщение: %s", buffer);
    }

    // Закрыть сокет
    close(clientsockfd);

    return 0;
}
