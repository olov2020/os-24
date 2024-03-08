#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int transmitter_pid;
int received_num = 0;
int bit_count = 0;

void sigusr1_handler(int signum) {
    received_num |= (1 << bit_count);
    bit_count++;
    if (bit_count == 32) {
        printf("Received number: %d\n", received_num);
        _exit(0); // Exit after receiving all bits
    }
    kill(transmitter_pid, SIGUSR1);
}

void sigusr2_handler(int signum) {
    bit_count++;
    if (bit_count == 32) {
        printf("Received number: %d\n", received_num);
        _exit(0); // Exit after receiving all bits
    }
    kill(transmitter_pid, SIGUSR1);
}

int main() {
    printf("Receiver PID: %d\n", getpid());
    
    printf("Enter the PID of the transmitter: ");
    scanf("%d", &transmitter_pid);

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);

    pause(); // Wait for the first bit

    printf("Transmission completed!\n");

    return 0;
}
