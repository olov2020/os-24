#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    struct sockaddr_in serv_addr;
    
    int client_fd;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    
    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Дикарь присоединился к обеду\n");

    // Логика клиента (дикаря)
    char buffer[1024] = {0};
    int valread;

    // Отправляем запрос на сервер
    send(client_fd, "Запрос: Хочу съесть кусок миссионера", 35, 0);
    
    // Получаем ответ от сервера
    valread = read(client_fd, buffer, 1024);
    printf("Ответ от повара: %s\n", buffer);

    // Закрытие клиентского сокета
    close(client_fd);

    return 0;
}
