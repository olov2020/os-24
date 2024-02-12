#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];

    int input_fd = open(input_file, O_RDONLY);
    if (input_fd == -1) {
        perror("Error opening input file");
        return 1;
    }

    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (output_fd == -1) {
        perror("Error opening output file");
        close(input_fd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(output_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("Error writing to output file");
            close(input_fd);
            close(output_fd);
            return 1;
        }
    }

    if (bytes_read == -1) {
        perror("Error reading input file");
        close(input_fd);
        close(output_fd);
        return 1;
    }

    close(input_fd);
    close(output_fd);

    printf("File copied successfully!\n");

    return 0;
}
