#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 64

int main() {
    char input_file[256];
    char output_file[256];

    printf("Enter input file name: ");
    scanf("%s", input_file);

    printf("Enter output file name: ");
    scanf("%s", output_file);

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
    int total_bytes_written = 0;

    while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
        int bytes_to_write = bytes_read;
        int offset = 0;

        while (bytes_to_write > 0) {
            bytes_written = write(output_fd, buffer + offset, bytes_to_write);
            if (bytes_written == -1) {
                perror("Error writing to output file");
                close(input_fd);
                close(output_fd);
                return 1;
            }

            bytes_to_write -= bytes_written;
            offset += bytes_written;
            total_bytes_written += bytes_written;
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

    printf("Data copied successfully from %s to %s! Total bytes written: %d\n", input_file, output_file, total_bytes_written);

    return 0;
}
