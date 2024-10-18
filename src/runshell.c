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

    int input_buffer_size = MAX_STR_SIZE;
    char read_buffer[input_buffer_size];

    int read_result;
    int is_shell_running = true;

	Task tasks[MAX_TASKS];

	int current_task_count = 0;

    while(is_shell_running){
		print_string(shell_name,false);
		read_result = read_input(read_buffer, input_buffer_size);
		parse(read_buffer, tasks, &current_task_count);
		link_tasks(tasks, current_task_count);

		if(kstrcmp(read_buffer,exit_command) == true){
			is_shell_running = false;
		}else{
			run_task_tree(tasks, current_task_count);
		}
		current_task_count = 0;
    }

    return 0;
}