#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>

#include "defs.h"

// Function to get the value of an environment variable
int get_env_value(char *var_name, char **env_value) {
    int fd;
    char buffer[ENV_BUFFER];
    ssize_t bytesRead;
    char *var_start = NULL;


    int var_len = kstrlen(var_name);  // Use custom function to get the length of var_name


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

    // Traverse through environment variables in the buffer
    for (int i = 0; i < bytesRead; ) {
        // Compare the current variable name with var_name
        if (kstrcmp_by_n(&buffer[i], var_name, var_len) == true && buffer[i + var_len] == '=') {
            var_start = &buffer[i + var_len + 1];  // Skip "var_name="
            break;
        }
        
        // Move to the next environment variable by skipping over the null-terminated string
        while (i < bytesRead && buffer[i] != '\0') {
            i++;
        }
        i++;  // Move past the null terminator to the next environment variable
    }

    if (var_start == NULL) {
        return -1;  // Specified environment variable not found
    }

    // Calculate the length of the environment variable value (until the next '\0')
    int j = 0;
    while (var_start[j] != '\0' && (var_start + j) < buffer + bytesRead) {
        j++;
    }

    // Set the env_value to point to the start of the environment variable's value
    *env_value = var_start;

    // Return the length of the variable's value
    return j;
}