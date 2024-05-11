#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *end_message = "The End";
    char buffer[1024];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while (1) {
        read(sock, buffer, 1024);

        if (strcmp(buffer, end_message) == 0) {
            printf("End of communication\n");
            break;
        }

        printf("Client 1: %s\n", buffer);

        printf("Enter message for Client 1: ");
        fgets(buffer, 1024, stdin);
        send(sock, buffer, strlen(buffer), 0);

        if (strcmp(buffer, end_message) == 0) {
            printf("End of communication\n");
            break;
        }
    }

    close(sock);
    return 0;
}
