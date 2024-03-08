#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int transmitter_pid;
int received_number = 0;
int bit_counter = 0;

void sigusr1_handler(int signo) {
    // обработчик сигнала SIGUSR1
    received_number |= (1 << bit_counter);
    bit_counter++;
    printf("Received bit 1\n");
    if (bit_counter < 32) {
        kill(transmitter_pid, SIGUSR1);
    } else {
        printf("Received number: %d\n", received_number);
        exit(0);
    }
}

void sigusr2_handler(int signo) {
    // обработчик сигнала SIGUSR2
    printf("Received bit 0\n");
    bit_counter++;
    if (bit_counter < 32) {
        kill(transmitter_pid, SIGUSR1);
    } else {
        printf("Received number: %d\n", received_number);
        exit(0);
    }
}

int main() {
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);

    printf("Receiver PID: %d\n", getpid());
    printf("Enter transmitter PID: ");
    scanf("%d", &transmitter_pid);

    kill(transmitter_pid, SIGUSR1);

    while(1) {
        pause();
    }

    return 0;
}
