#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    char server_ip[256];

    // Ввести IP-адрес сервера
    printf("Введите IP-адрес сервера: ");
    scanf("%s", server_ip);

    // Создать сокет
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Подключиться к серверу
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = inet_addr(server_ip);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Ждать сообщения от повара о том, что горшок полон
        char msg[256];
        recv(sockfd, msg, sizeof(msg), 0);

        // Если горшок полон, есть один кусок мяса
        if (strcmp(msg, "Горшок полон") == 0) {
            printf("Дикарь поелn");
        }
    }

    close(sockfd);

    return 0;
}
