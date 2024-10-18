#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "defs.h"
#include "utils.h"

#include "basicio.h"
int main() {
    char env_value[2000];
    print_string("Type in an environment Variable: ", false);

    int read_result;
    int input_buffer_size = MAX_STR_SIZE;
    char read_buffer[input_buffer_size];
    read_result = read_input(read_buffer, input_buffer_size);

    // Remove newline character from read_buffer if present
    if (read_result > 0 && read_buffer[read_result - 1] == '\n') {
        read_buffer[read_result - 1] = '\0';
    } else {
        read_buffer[read_result] = '\0';
    }

    int value_len = get_env_value(read_buffer, env_value);

    if (value_len > 0) {
        print_string(read_buffer, false);
        print_string("=", false);
        print_string(env_value, true);
    } else {
        write(STDERR_FILENO, "Environment variable not found\n", 31);
    }

    return 0;
}


// this does not work
