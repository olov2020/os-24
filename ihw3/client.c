#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    struct sockaddr_in serv_addr;
    
    int client_fd;
    int meat_left;

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
    
    // Логика клиента (дикаря)
    while(1) {
        recv(client_fd, &meat_left, sizeof(meat_left), 0);
        
        if (meat_left <= 0) {
            printf("Дикарь ждет, пока повар приготовит еду\n");
        } else {
            printf("Дикарь пообедал. Осталось %d кусков мяса\n", meat_left);
        }
        
        sleep(1); // Задержка перед следующим запросом на кусок мяса
    }

    return 0;
}
