#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "defs.h"
#include "utils.h"
#include "basicio.h"

int main() {
    char command[256];
    char resolved_path[256];

    print_string("Enter a command to resolve (e.g., ls): ", false);

    int read_result;
    read_result = read_input(command, sizeof(command));

    // Remove newline character from the input, if present
    if (read_result > 0 && command[read_result - 1] == '\n') {
        command[read_result - 1] = '\0';
    }

    int result = check_env_path(command, resolved_path);

    if (result == 1) {
        print_string("Command found: ", false);
        print_string(resolved_path, true);
    } else if (result == 0) {
        print_string("Using provided path: ", false);
        print_string(resolved_path, true);
    } else {
        print_string("Command not found in PATH", true);
    }

    return EXIT_SUCCESS;
}