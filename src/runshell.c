#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>

#include "basicio.h"
#include "shell.h"
#include "defs.h"
#include "process.h"


int main(int argc, char *argv[]) {
    int input_buffer_size = 256;
    char test[input_buffer_size];
    int result;
    result = read_input(test, input_buffer_size);
    print_string(test);

    char *command = "/bin/ls";  // Corrected path if it's Linux
    char *args[] = {command, "-al", NULL};
    char *env[] = {NULL};

    int E = run_process(command, args, env, true);

    if(E == -1){
    	print_string("Error");
    }

    return 0;
}