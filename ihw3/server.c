#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;
    int meat_left = 0; // Количество кусков мяса в горшке

    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Установка параметров сокета
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Привязка адреса и порта к сокету
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Прослушивание подключений
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Принятие нового соединения
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Логика обслуживания клиентов (дикарей)
    while(1) {
        if (meat_left == 0) {
            // Повар ждет, пока его не разбудят
            sleep(5); // Время на приготовление еды
            meat_left = 10; // Повар наполняет горшок
            printf("Повар наполнил горшок\n");
        }
        
        // Отправка количества кусков мяса клиенту
        send(new_socket, &meat_left, sizeof(meat_left), 0);
        
        meat_left--; // Дикарь съел один кусок мяса
        printf("Дикарь пообедал. Осталось %d кусков мяса\n", meat_left);
    }

    return 0;
}
