#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>

unsigned long long int factorial(unsigned int n) {
    unsigned long long int result = 1;
    for (unsigned int i = 1; i <= n; ++i) {
        if (result > ULLONG_MAX / i) {
            fprintf(stderr, "overflow!\n");
            exit(1);
        }
        result *= i;
    }
    return result;
}

unsigned long long int fibonacci(unsigned int n) {
    unsigned long long int a = 0, b = 1;
    for (unsigned int i = 0; i < n; ++i) {
        if (a > ULLONG_MAX - b) {
            fprintf(stderr, "Overflow!\n");
            exit(1);
        }
        unsigned long long int temp = a + b;
        a = b;
        b = temp;
    }
    return a;
}

void print_process_info() {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    printf("Process ID: %d\n", pid);
    printf("Parent Process ID: %d\n", ppid);
}

void print_directory_contents() {
    printf("Current Directory Contents:\n");
    system("ls -al");
}

int main(int argc, char* argv[]) {

    print_process_info();

    unsigned int n;
    printf("Enter number: ");
    scanf("%u", &n);

    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        print_process_info();
        unsigned long long int result = factorial(n);
        printf("Factorial %u = %llu\n", n, result);
    } else if (pid > 0) {
        // Parent process
        print_process_info();
        unsigned long long int result = fibonacci(n);
        printf("Fibonacci %u = %llu\n", n, result);

        pid_t wait_pid;
        int status;
        wait_pid = wait(&status);
        if (wait_pid == -1) {
            fprintf(stderr, "Error occurred while waiting for child process\n");
            return 1;
        }

        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process terminated by signal %d\n", WTERMSIG(status));
        }

        print_directory_contents();
    } else {
        fprintf(stderr, "Error creating child process!\n");
        return 1;
    }

    return 0;
}
