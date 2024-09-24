
#include <unistd.h>
#include <fcntl.h>

#include "defs.h"

#define MAX_ARGS 128      // Maximum number of arguments
#define BUFFER_SIZE 1024  // Size of the buffer to hold the arguments

char args_buffer[BUFFER_SIZE];   // Buffer to store the command arguments
char *argv[MAX_ARGS];            // Array of pointers to the arguments

int get_args(int *argc, char *command) {
    if (argc == NULL || command == NULL) {
        return -1; // Error: Null pointer
    }

    *argc = 0;  // Initialize the argument count
    char *p = command;
    int buffer_index = 0;

    while (*p != '\0') {
        // Skip leading spaces
        while (*p == ' ' || *p == '\t') {
            p++;
        }

        if (*p == '\0') {
            break; // End of string
        }

        // Check if there is enough space for another argument
        if (*argc >= MAX_ARGS || buffer_index >= BUFFER_SIZE) {
            return -1; // Error: Too many arguments or buffer overflow
        }

        argv[*argc] = &args_buffer[buffer_index];  // Point to the location in the buffer

        // Copy the argument into the buffer
        while (*p != '\0' && *p != ' ' && *p != '\t') {
            args_buffer[buffer_index++] = *p++;
            if (buffer_index >= BUFFER_SIZE) {
                return -1; // Buffer overflow
            }
        }
        args_buffer[buffer_index++] = '\0'; // Null-terminate the current argument
        (*argc)++; // Increment the argument count
    }

    return 0; // Success
}
