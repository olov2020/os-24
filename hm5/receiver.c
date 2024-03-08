#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int transmitter_pid;

void sigusr1_handler(int signum) {
    printf("Received 1\n");
    kill(transmitter_pid, SIGUSR1);
}

void sigusr2_handler(int signum) {
    printf("Received 0\n");
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
