#include <unistd.h>
#include <fcntl.h>

#include "basicio.h"
#include "str.h"
#include "defs.h"
#include "utils.h"


int get_env_value(char *var_name, char *env_value) {
    int fd;
    char buffer[ENV_BUFFER];
    ssize_t bytesRead;
    char *var_start = NULL;
    int var_len = kstrlen(var_name);  // Use custom function to get the length of var_name

    // Open /proc/self/environ file to access the en/vironment variables
    fd = open("/proc/self/environ", O_RDONLY);
    if (fd < 0) {
        print_error("utils.c: open() failed");
        return EXIT_FAILURE;  // Error opening the file
    }

    // Read the environment variables into buffer
    bytesRead = read(fd, buffer, ENV_BUFFER);
    if (bytesRead < 0) {
        close(fd);
        print_error("utils.c: read() failed");
        return EXIT_FAILURE;  // Error reading the file
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
        print_error("utils.c: specified environment variable not found");
        return EXIT_FAILURE; 
    }

    // Copy the environment variable value into env_value
    int j = 0;
    while (var_start[j] != '\0' && (var_start + j) < buffer + bytesRead) {
        env_value[j] = var_start[j];
        j++;
    }
    env_value[j] = '\0'; // Null-terminate the env_value string

    // Return the length of the variable's value
    return j;
}


int check_env_path(char *old_path, char *new_path) {
    if (kstrhas_unary(old_path, '/')) {
        kstrcpy(old_path, new_path);
        return EXIT_SUCCESS; // No change needed
    } else {
        char path_value[ENV_BUFFER];
        int path_len = get_env_value("PATH", path_value);
        if (path_len < 0) {
            print_error("utils.c: get_env_value() failed");
            return EXIT_FAILURE; // Error retrieving PATH
        }

        int i = 0, dir_start = 0;
        char full_path[MAX_CMD_LENGTH];

        while (1) {
            char ch = path_value[i];
            if (ch == ':' || ch == '\0') {
                // Temporarily null-terminate the directory name
                path_value[i] = '\0';

                char *dir = &path_value[dir_start];

                full_path[0] = '\0';          // Initialize full_path
                kstrcpy(dir, full_path);      // Copy directory into full_path
                kstrconcat(full_path, "/");   // Append '/'
                kstrconcat(full_path, old_path); // Append old_path

                int fd = open(full_path, O_RDONLY);
                if (fd >= 0) {
                    close(fd); // Close file descriptor if file exists
                    kstrcpy(full_path, new_path); // Copy full_path to new_path
                    return 1; // Change made
                }

                // Restore the original character
                path_value[i] = ch;

                if (ch == '\0') {
                    break; // End of PATH reached
                }
                dir_start = i + 1;
            }
            i++;
        }
        print_error("utils.c: command not found in PATH");
        return EXIT_FAILURE;
    }
}
