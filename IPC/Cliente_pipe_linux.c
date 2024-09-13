#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define PIPE_NUMERIC "/tmp/numbers_pipe"
#define PIPE_STRING "/tmp/strings_pipe"

int main() {
    int pipe_fd;
    char buffer[256];
    
    // Sending a numeric request
    pipe_fd = open(PIPE_NUMERIC, O_WRONLY);
    sprintf(buffer, "10 20"); // Request to sum 10 and 20
    write(pipe_fd, buffer, strlen(buffer) + 1);
    close(pipe_fd);

    // Sending a string request
    pipe_fd = open(PIPE_STRING, O_WRONLY);
    sprintf(buffer, "Hello World!"); // String request
    write(pipe_fd, buffer, strlen(buffer) + 1);
    close(pipe_fd);

    return 0;
}
