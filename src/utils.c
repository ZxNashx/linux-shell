#include <unistd.h>
#include <fcntl.h>

#include "defs.h"

int get_path_env(char **env_value) {
    int fd;
    char buffer[ENV_BUFFER];
    ssize_t bytesRead;
    char *path_start = NULL;

    // Open /proc/self/environ file to access the environment variables
    fd = open("/proc/self/environ", O_RDONLY);
    if (fd < 0) {
        return -1;  // Error opening the file
    }

    // Read the environment variables into buffer
    bytesRead = read(fd, buffer, ENV_BUFFER);
    if (bytesRead < 0) {
        close(fd);
        return -1;  // Error reading the file
    }

    // Close the file descriptor
    close(fd);

    // Search for the PATH variable in the buffer
    for (char *current = buffer; current < buffer + bytesRead; current++) {
        if (current[0] == 'P' && current[1] == 'A' && current[2] == 'T' && current[3] == 'H' && current[4] == '=') {
            path_start = current + 5;  // Skip "PATH="
            break;
        }
        // Move to the next environment variable (skipping '\0')
        while (current < buffer + bytesRead && *current != '\0') {
            current++;
        }
    }

    if (path_start == NULL) {
        return -1;  // PATH variable not found
    }

    // Allocate memory and copy the PATH value to the provided string pointer
    *env_value = path_start;
    return 0;  // Success
}