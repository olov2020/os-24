#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_CLIENTS 5

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length;
    int optval = 1;
    int client_count = 0;

    // Создание сокета
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Ошибка создания сокета");
        exit(1);
    }

    // Настройка сокета
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // Заполнение структуры адреса
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Привязка сокета к адресу
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Ошибка привязки сокета");
        close(server_socket);
        exit(1);
    }

    // Прослушивание на сокете
    if (listen(server_socket, 5) == -1) {
        perror("Ошибка прослушивания на сокете");
        close(server_socket);
        exit(1);
    }

    printf("Сервер запущен на порту %d\n", PORT);

    // Прием клиентов
    while (1) {
        // Ожидание подключения
        client_address_length = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);
        if (client_socket == -1) {
            perror("Ошибка приема клиента");
            continue;
        }

        // Проверка ограничения на количество клиентов
        if (client_count >= MAX_CLIENTS) {
            printf("Достигнут лимит клиентов.\n");
            close(client_socket);
            continue;
        }

        // Прием запроса от клиента
        char request[1024];
        int bytes_read = recv(client_socket, request, sizeof(request), 0);
        if (bytes_read == -1) {
            perror("Ошибка приема запроса");
            close(client_socket);
            continue;
        }
        request[bytes_read] = '\0';

        // Обработка запроса
        printf("Дикарь %d: %s\n", client_count + 1, request);

        // Отправка ответа клиенту
        char response[1024] = "Привет, дикарь!\n";
        send(client_socket, response, strlen(response), 0);

        // Закрытие соединения с клиентом
        close(client_socket);

        // Увеличение счетчика клиентов
        client_count++;
    }

    // Закрытие серверного сокета
    close(server_socket);

    return 0;
}
