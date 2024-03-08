#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int receiver_pid;

void sigusr1_handler(int signo) {
    // обработчик сигнала SIGUSR1
    printf("Received ACK from receiver\n");
}

void sigusr2_handler(int signo) {
    // обработчик сигнала SIGUSR2
    printf("Received NACK from receiver, resending last bit\n");
    kill(receiver_pid, SIGUSR1);
}

int main() {
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);

    printf("Transmitter PID: %d\n", getpid());
    printf("Enter receiver PID: ");
    scanf("%d", &receiver_pid);

    printf("Enter a decimal number to transmit: ");
    int number;
    scanf("%d", &number);

    for (int i = 31; i >= 0; i--) {
        int bit = (number >> i) & 1;
        if (bit == 1) {
            kill(receiver_pid, SIGUSR1);
        } else {
            kill(receiver_pid, SIGUSR2);
        }
        pause();
    }

    return 0;
}
