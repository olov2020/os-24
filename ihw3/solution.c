```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define MEAT_PIECES 10

// Структура для хранения информации о дикаре
typedef struct {
    int id;
    int sockfd;
} Savage;

// Глобальные переменные
int num_savages = 0;
Savage savages[MAX_CLIENTS];
sem_t pot_empty;
sem_t pot_full;
int pot_pieces = 0;

// Функция обработки клиента (дикаря)
void *savage_handler(void *arg) {
    Savage *savage = (Savage *)arg;
    char buffer[100];
    int bytes_read;

    while (1) {
        // Проверяем, есть ли в горшке еда
        sem_wait(&pot_empty);

        // Если в горшке есть еда, едим
        if (pot_pieces > 0) {
            pot_pieces--;
            sprintf(buffer, "Дикарь %d съел кусок мяса.\n", savage->id);
            write(savage->sockfd, buffer, strlen(buffer));
        } else {
            // Если горшок пуст, будим повара
            sprintf(buffer, "Дикарь %d будит повара.\n", savage->id);
            write(savage->sockfd, buffer, strlen(buffer));
            sem_post(&pot_full);
        }

        // Ждем повара
        sem_wait(&pot_full);

        // Читаем сообщение от повара
        bytes_read = read(savage->sockfd, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            break;
        }

        printf("%s", buffer);
    }

    close(savage->sockfd);
    pthread_exit(NULL);
}

// Функция обработки повара
void *cook_handler(void *arg) {
    int sockfd, client_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    pthread_t thread;

    // Создание сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Ошибка создания сокета");
        exit(1);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Привязка сокета к адресу
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Ошибка привязки сокета");
        exit(1);
    }

    // Слушаем соединения
    if (listen(sockfd, 5) == -1) {
        perror("Ошибка прослушивания сокета");
        exit(1);
    }

    // Цикл обработки клиентов
    while (1) {
        client_addr_len = sizeof(client_addr);
        client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sockfd == -1) {
            perror("Ошибка приема соединения");
            continue;
        }

        // Проверка максимального количества клиентов
        if (num_savages >= MAX_CLIENTS) {
            char buffer[100];
            sprintf(buffer, "Слишком много дикарей! Больше не могу принять.\n");
            write(client_sockfd, buffer, strlen(buffer));
            close(client_sockfd);
            continue;
        }

        // Добавление нового дикаря
        savages[num_savages].sockfd = client_sockfd;
        savages[num_savages].id = num_savages + 1;
        num_savages++;

        // Создание потока для обработки дикаря
        if (pthread_create(&thread, NULL, savage_handler, &savages[num_savages - 1])) {
            perror("Ошибка создания потока");
            exit(1);
        }

        printf("Дикарь %d присоединился к обеду.\n", num_savages);
    }

    close(sockfd);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int max_savages;
    pthread_t cook_thread;

    // Получение максимального количества дикарей из аргументов командной строки
    if (argc != 2) {
        printf("Использование: %s <максимальное_количество_дикарей>\n", argv[0]);
        exit(1);
    }

    max_savages = atoi(argv[1]);
    if (max_savages <= 0 || max_savages > MAX_CLIENTS) {
        printf("Максимальное количество дикарей должно быть от 1 до %d.\n", MAX_CLIENTS);
        exit(1);
    }

    // Инициализация семафоров
    seminit(&potempty, 0, MEATPIECES);
    seminit(&potfull, 0, 0);

    // Создание потока для повара
    if (pthreadcreate(&cookthread, NULL, cookhandler, NULL)) {
        perror("Ошибка создания потока повара");
        exit(1);
    }

    // Ждем завершения повара
    pthreadjoin(cookthread, NULL);

    // Удаление семафоров
    semdestroy(&potempty);
    semdestroy(&potfull);

    return 0;
}
