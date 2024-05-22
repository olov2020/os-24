#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int client_socket;
    struct sockaddr_in server_address;

    // Создание сокета
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Ошибка создания сокета");
        exit(1);
    }

    // Заполнение структуры адреса
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_HOST);
    server_address.sin_port = htons(SERVERPORT);

    // Подключение к серверу
    if (connect(clientsocket, (struct sockaddr*)&serveraddress, sizeof(serveraddress)) == -1) {
        perror("Ошибка подключения к серверу");
        close(clientsocket);
        exit(1);
    }

    // Отправка запроса на сервер
    char request[1024] = "Я голодный!";
    send(clientsocket, request, strlen(request), 0);

    // Прием ответа от сервера
    char response[1024];
    int bytesread = recv(clientsocket, response, sizeof(response), 0);
    if (bytesread == -1) {
        perror("Ошибка приема ответа");
        close(clientsocket);
        exit(1);
    }
    response[bytesread] = '\0';

    // Вывод ответа
    printf("Ответ сервера: %s\n", response);

    // Закрытие клиентского сокета
    close(clientsocket);

    return 0;
}
