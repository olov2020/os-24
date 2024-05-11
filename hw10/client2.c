#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <serveripaddress> <serverport>\n", argv[0]);
        return EXITFAILURE;
    }

    // Создание сокета клиента
    int clientsocket = socket(AFINET, SOCKSTREAM, 0);
    if (clientsocket == -1) {
        perror("socket");
        return EXITFAILURE;
    }

    // Настройка адреса сервера
    struct sockaddrin serveraddress;
    memset(&serveraddress, 0, sizeof(serveraddress));
    serveraddress.sinfamily = AFINET;
    serveraddress.sinaddr.saddr = inetaddr(argv[1]);
    serveraddress.sinport = htons(atoi(argv[2]));

    // Соединение с сервером
    if (connect(clientsocket, (struct sockaddr )&serveraddress, sizeof(serveraddress)) == -1) {
        perror("connect");
        return EXITFAILURE;
    }

    // Получение и вывод сообщений
    char buffer[1024];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        if (recv(clientsocket, buffer, sizeof(buffer), 0) <= 0) {
            break;
        }
        printf("Received message: %s", buffer);
    }

    // Закрываем сокет клиента
    close(clientsocket);

    return EXITSUCCESS;
}
