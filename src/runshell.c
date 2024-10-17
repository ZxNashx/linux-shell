#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>

#include "basicio.h"
#include "shell.h"
#include "defs.h"
#include "task.h"
#include "process.h"
#include "parser.h"
#include "str.h"

int main(int argc, char *argv[]) {
	char *shell_name = "$= ";
	char *exit_command = "exit";
	char *jobs_command = "jobs";
	

	char *command = "/bin/ls";
    char *args[MAX_CMD_TOKENS];
    char *env[] = {NULL};

    int input_buffer_size = MAX_STR_SIZE;
    char read_buffer[input_buffer_size];
	char original_input_buffer[input_buffer_size];
    int read_result;
    int is_shell_running = true;

		Task test_task;

    char * cmd_tokens[MAX_CMD_TOKENS];
	int curr_token = 0;
    int token_count = 0;
    char to_split = ' ';
	bool is_shell_cmd;

    while(is_shell_running){
		print_string(shell_name,false);
		read_result = read_input(read_buffer, input_buffer_size);
		print_string("Entered: ",false);
		print_string(read_buffer,true);

		int split_str_result = split_str(read_buffer, cmd_tokens, &token_count, to_split, original_input_buffer);
		is_shell_cmd = false;
		if(kstrcmp(read_buffer,exit_command) == true){
			is_shell_cmd = true;
			is_shell_running = false;
		}

		command = cmd_tokens[curr_token];

		args[token_count] = NULL; // Set the final string to NULL or an empty string

		printf("DEBUG:\nEntered: %s\nResult: %d\nToken count %d\n", read_buffer, read_result, token_count);
        if (!is_shell_cmd) {
					if(split_str_result > 1){
						cmd_tokens[token_count] = NULL;
						int pr = run_process(command, cmd_tokens, env, true, STDIN_FILENO, STDOUT_FILENO);
						printf("process results: %d\n", pr);
					}else{
						cmd_tokens[token_count] = NULL;
						command = cmd_tokens[0];
						int pr = run_process(command, cmd_tokens, env, true, STDIN_FILENO, STDOUT_FILENO);
						printf("process results: %d\n", pr);
					}
        }
    }

    return 0;
}