#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSZ 27

int main()
{
    key_t key;
    int shmid;
    char *shm, *s;

    key = ftok(".", 'B');

    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    s = shm;

    for (s = shm; *s != 0; s++)
    {
        printf("%d\n", (int)*s);
    }

    // Устанавливаем флаг завершения для клиента
    *shm = '*';

    // Отсоединяемся от сегмента разделяемой памяти
    shmdt(shm);

    // Удаляем сегмент разделяемой памяти
    shmctl(shmid, IPC_RMID, NULL);

    exit(0);
}
