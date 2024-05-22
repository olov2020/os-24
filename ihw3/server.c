#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main() {
    struct sockaddr_in serv_addr, cli_addr;
    int server_fd, client_fd;
    int opt = 1;
    int addrlen = sizeof(serv_addr);
    int max_clients = 5; // Максимальное количество дикарей

    char buffer[1024] = {0};
    char *response = "Ответ: Повар дал кусок миссионера";

    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Установка опции переиспользования порта
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(8080);

    // Привязка сокета к адресу и порту
    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Прослушивание подключений
    if (listen(server_fd, max_clients) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    while(1) {
        // Принятие входящих подключений
        if ((client_fd = accept(server_fd, (struct sockaddr *)&cli_addr, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Повар принял заказ\n");

        // Получение запроса от клиента
        read(client_fd, buffer, 1024);
        printf("Запрос от дикаря: %s\n", buffer);

        // Отправка ответа клиенту
        send(client_fd, response, strlen(response), 0);

        // Закрытие сокета клиента
        close(client_fd);
    }

    // Закрытие сокета сервера
    close(server_fd);

    return 0;
}
