#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "defs.h"
#include "utils.h"

int main() {
    char *env_value;
    char var_name[] = "PATH";  // You can change this to any environment variable name

    int value_len = get_env_value(var_name, &env_value);

    if (value_len > 0) {
        write(STDOUT_FILENO, var_name, kstrlen(var_name));  // Print variable name
        write(STDOUT_FILENO, "=", 1);
        write(STDOUT_FILENO, env_value, value_len);  // Print variable value
        write(STDOUT_FILENO, "\n", 1);
    } else {
        write(STDERR_FILENO, "Environment variable not found\n", 31);
    }

    return 0;
}

// this does not work
