#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Максимальное количество дикарей
#define MAX_SAVAGES 5

// Семафор, защищающий доступ к горшку
sem_t pot_mutex;
// Семафор, сигналящий о том, что горшок пуст
sem_t pot_empty;
// Семафор, сигналящий о том, что горшок полон
sem_t pot_full;

// Общее количество кусков мяса в горшке
int pot_count = 0;

// Поток дикаря
void *savage(void *arg) {
    int savage_id = (int)arg;
    int sockfd = *(int *)arg;

    while (1) {
        // Захватить семафор горшка
        sem_wait(&pot_mutex);

        // Если горшок пуст, разбудить повара
        if (pot_count == 0) {
            sem_post(&pot_empty);
        }

        // Есть один кусок мяса из горшка
        pot_count--;

        // Освободить семафор горшка
        sem_post(&pot_mutex);

        // Отправить сообщение повару о том, что дикарь поел
        char msg[256];
        sprintf(msg, "Дикарь %d поел", savage_id);
        send(sockfd, msg, strlen(msg) + 1, 0);

        // Подождать немного прежде, чем снова захотеть есть
        sleep(1);
    }

    close(sockfd);

    return NULL;
}

int main() {
    int num_savages;

    // Ввести количество дикарей
    printf("Введите количество дикарей (не более %d): ", MAX_SAVAGES);
    scanf("%d", &num_savages);
    if (num_savages > MAX_SAVAGES) {
        fprintf(stderr, "Error: number of savages cannot exceed %dn", MAX_SAVAGES);
        exit(EXIT_FAILURE);
    }

    // Инициализировать семафоры
    sem_init(&pot_mutex, 0, 1);
    sem_init(&pot_empty, 0, 0);
    sem_init(&pot_full, 0, 0);

    // Создать сокет
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Привязать сокет к адресу
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Начать прослушивание
    if (listen(sockfd, num_savages) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Создать потоки дикарей
    pthread_t savage_threads[num_savages];
    int savage_sockets[num_savages];
    for (int i = 0; i < num_savages; i++) {
        // Принять подключение от дикаря
        int *savage_sockfd = malloc(sizeof(int));
        *savage_sockfd = accept(sockfd, NULL, NULL);
        if (*savage_sockfd < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        savage_sockets[i] = *savage_sockfd;

        // Создать поток для дикаря
        pthread_create(&savage_threads[i], NULL, savage, (void *)savage_sockfd);
    }

    // Поток повара
    while (1) {
        // Ждать, пока дикарь не разбудит повара
        sem_wait(&pot_empty);

        // Наполнить горшок
        pot_count = 5;

        // Сигнализировать о том, что горшок полон
        sem_post(&pot_full);

        // Отправить сообщение всем дикарям о том, что горшок полон
        char msg[256];
        strcpy(msg, "Горшок полон");
        for (int i = 0; i < num_savages; i++) {
            send(savage_sockets[i], msg, strlen(msg) + 1, 0);
        }

        // Снова заснуть
        printf("Повар сварил обедn");
        sleep(1);
    }

    // Присоединить все потоки
    for (int i = 0; i < num_savages; i++) {
        pthread_join(savage_threads[i], NULL);
    }

    // Уничтожить семафоры
    sem_destroy(&pot_mutex);
    sem_destroy(&pot_empty);
    sem_destroy(&pot_full);

    // Закрыть сокет
    close(sockfd);

    return 0;
}
