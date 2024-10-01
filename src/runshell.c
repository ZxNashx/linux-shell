#include <unistd.h>
#include <fcntl.h>

//#include <stdio.h>

#include "basicio.h"
#include "shell.h"
#include "defs.h"
#include "process.h"
#include "str.h"

int main(int argc, char *argv[]) {
	char *shell_name = "$= ";
	char *exit_command = "exit";

	char *command = "/bin/ls";
    char *args[MAX_CMD_TOKENS];
    char *env[] = {NULL};

    int input_buffer_size = MAX_STR_SIZE;
    char read_buffer[input_buffer_size];
    int read_result;
    int is_shell_running = true;

    char * cmd_tokens[MAX_CMD_TOKENS];
    int token_count = 0;
    char to_split = ' ';
	bool is_shell_cmd;

    while(is_shell_running){
		print_string(shell_name,false);
		read_result = read_input(read_buffer, input_buffer_size);
		split_str(read_buffer, cmd_tokens, &token_count, to_split);
		
		print_string("Entered: ",false);
		print_string(read_buffer,true);

		is_shell_cmd = false;
		if(strcmp(read_buffer,exit_command) == true){
			is_shell_cmd = true;
			is_shell_running = false;
		}else{
			is_shell_cmd = false;
		}

		for(int i = token_count; i >= 0; i--){
			args[i] = cmd_tokens[i]; // First argument (if any)
		}

		command = cmd_tokens[0];

		//printf("DEBUG:\nEntered: %s\nResult: %d\nToken count %d\n", read_buffer, read_result, token_count);
		if(!is_shell_cmd){
			//printf("Command:\ncmd: \n%s\nargs: %s\n",cmd_tokens[0],cmd_tokens[1]);
			int pr = run_process(command, args, env, true);
			//printf("process results: %d\n");
		}
    }

    return 0;
}