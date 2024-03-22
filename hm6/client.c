#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

#define SHMSZ 27

#include <signal.h>

void handle_signal(int signal)
{
    if (signal == SIGINT)
    {
        // Удаление сегмента разделяемой памяти
        shmctl(shmid, IPC_RMID, NULL);
        exit(0);
    }
}

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

    srand(getpid());

    int random_num = rand() % 1000 + 1;

    *s = random_num;

    while (*shm != '*')
    {
        sleep(1);
    }

    signal(SIGINT, handle_signal);
}
