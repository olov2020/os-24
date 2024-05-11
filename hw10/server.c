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

    // Создание сокета сервера
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    // Настройка адреса сервера
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));

    // Привязка сокета к адресу
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        return EXIT_FAILURE;
    }

    // Слушаем входящие подключения
    if (listen(server_socket, 5) == -1) {
        perror("listen");
        return EXIT_FAILURE;
    }

    while (1) {
        // Принимаем соединение от клиента №1
        int client1_socket = accept(server_socket, NULL, NULL);
        if (client1_socket == -1) {
            perror("accept");
            continue;
        }

        // Принимаем соединение от клиента №2
        int client2_socket = accept(server_socket, NULL, NULL);
        if (client2_socket == -1) {
            perror("accept");
            close(client1_socket);
            continue;
        }

        // Создаем поток для перенаправления сообщений
        int pid = fork();
        if (pid == 0) {
            // Поток для перенаправления сообщений
            char buffer[1024];

            // Перенаправляем сообщения от клиента №1 клиенту №2
            while (1) {
                memset(buffer, 0, sizeof(buffer));
                if (recv(client1_socket, buffer, sizeof(buffer), 0) <= 0) {
                    break;
                }
                send(client2_socket, buffer, strlen(buffer), 0);
            }

            // Закрываем сокеты
            close(client1_socket);
            close(client2_socket);

            exit(0);
        } else if (pid > 0) {
            // Родительский процесс, ожидающий завершения потока
            int status;
            waitpid(pid, &status, 0);
        } else {
            // Ошибка создания потока
            perror("fork");
            close(client1_socket);
            close(client2_socket);
        }
    }

    // Закрываем сокет сервера
    close(server_socket);

    return EXIT_SUCCESS;
}
