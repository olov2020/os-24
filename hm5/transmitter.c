#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int receiver_pid;

void sigusr1_handler(int signum) {
    printf("SIGUSR1 received, sending the next bit...\n");
    kill(receiver_pid, SIGUSR2);
}

void sigusr2_handler(int signum) {
    printf("SIGUSR2 received, sending the next bit...\n");
    kill(receiver_pid, SIGUSR1);
}

int main() {
    printf("Transmitter PID: %d\n", getpid());
    
    printf("Enter the PID of the receiver: ");
    scanf("%d", &receiver_pid);

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);

    int num;
    printf("Enter an integer to transmit: ");
    scanf("%d", &num);

    // Transmit each bit
    for (int i = 31; i >= 0; i--) {
        int bit = (num >> i) & 1;
        if (bit == 1) {
            kill(receiver_pid, SIGUSR1);
        } else {
            kill(receiver_pid, SIGUSR2);
        }
        pause(); // Wait for confirmation from the receiver
    }

    printf("Transmission completed! %d\n", num);
    
    return 0;
}
