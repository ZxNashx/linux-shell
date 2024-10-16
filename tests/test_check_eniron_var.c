#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "defs.h"
#include "utils.h"

#include "basicio.h"

int main() {
    char *env_value;
    print_string("Type in a environment Variable: ", false);

    int read_result;
    int input_buffer_size = MAX_STR_SIZE;
    char read_buffer[input_buffer_size];
    read_result = read_input(read_buffer, input_buffer_size);
    int value_len = get_env_value(read_buffer, &env_value);

    if (value_len > 0) {
        print_string(read_buffer,false);
        print_string("=",false);
        print_string(env_value, true);
    } else {
        write(STDERR_FILENO, "Environment variable not found\n", 31);
    }

    return 0;
}

// this does not work
