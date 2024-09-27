#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>

#include "basicio.h"
#include "shell.h"
#include "defs.h"
#include "process.h"
#include "str.h"

int main(int argc, char *argv[]) {
	char *shell_name = "$= ";

	char *exit_command = "exit";

	char *command = "/bin/ls";  // Corrected path if it's Linux
    char *args[] = {command, "-al", NULL};
    char *env[] = {NULL};

    int input_buffer_size = MAX_STR_SIZE;
    char read_buffer[input_buffer_size];
    int read_result;
    int is_shell_running = true;

    char * cmd_tokens[MAX_CMD_TOKENS];
    int token_count = 0;
    char to_split = ' ';


    while(is_shell_running){
		print_string(shell_name,false);
		read_result = read_input(read_buffer, input_buffer_size);
		split_str(read_buffer, cmd_tokens, &token_count, to_split);
		
		print_string("Entered: ",false);
		print_string(read_buffer,true);
		if(strcmp(read_buffer,exit_command) == true){
			is_shell_running = false;
		}else{

		}
		args[0] = cmd_tokens[0]; // Command
		args[1] = cmd_tokens[1]; // First argument (if any)
		args[2] = NULL;          // Null-terminate the argument list
		command = cmd_tokens[0];

		printf("DEBUG:\nEntered: %s\nResult: %d\nToken count %d\n", read_buffer, read_result, token_count);
		if(token_count > 1){
			printf("Command:\ncmd: \n%s\nargs: %s\n",cmd_tokens[0],cmd_tokens[1]);
			int pr = run_process(command, args, env, true);
			printf("process results: %d\n");
		}
    }

    return 0;
}