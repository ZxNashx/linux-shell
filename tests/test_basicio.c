#include <unistd.h>
#include <fcntl.h>

#include "basicio.h"
#include "defs.h"
#include "process.h"
#include "parser.h"
#include "str.h"

int main(int argc, char *argv[]) {
	char *shell_name = "$= ";
	char *exit_command = "exit";

    int input_buffer_size = MAX_STR_SIZE;
    char read_buffer[input_buffer_size];
	char original_buffer[input_buffer_size];
    int read_result;
    int is_shell_running = true;

    char * cmd_tokens[MAX_CMD_TOKENS];
    int token_count = 0;
    char to_split = ' ';
	bool is_shell_cmd;


	
    while(is_shell_running){
		print_string(shell_name,false);
		read_result = read_input(read_buffer, input_buffer_size);
		print_string("Entered: ",false);
		print_string(read_buffer,true);

		int split_str_result = split_str(read_buffer, cmd_tokens, &token_count, to_split, original_buffer);
		is_shell_cmd = false;
		if(kstrcmp(read_buffer,exit_command) == true){
			is_shell_cmd = true;
			is_shell_running = false;
		}
    }

    return EXIT_SUCCESS;
}