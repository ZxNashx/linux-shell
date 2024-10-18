#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "basicio.h"
#include "defs.h"
#include "task.h"
#include "process.h"
#include "parser.h"
#include "str.h"
#include "signal_handler.h"

int main(int argc, char *argv[]) {
	int input_buffer_size = MAX_STR_SIZE;
	int read_result = 0;
	int task_run_result = 0;
	int is_shell_running = true;
	int current_task_count = 0;
	Task tasks[MAX_TASKS];
	char read_buffer[input_buffer_size];

	struct sigaction sa = {
		.sa_handler = handle_sigint,
		.sa_flags = SA_RESTART | SA_NOCLDSTOP
	};

	// Block all signals during the handler execution
	sigemptyset(&sa.sa_mask);  			// Initialize the mask to be empty
	sigaddset(&sa.sa_mask, SIGINT);  	// Add SIGINT to the mask

	// Register the signal handler
	if (sigaction(SIGINT, &sa, NULL) == -1) {
		print_error("Signal handler setup error");
		is_shell_running = false;
	}

	print_string(WELCOME_MESSAGE, true);
    while (is_shell_running) {
		print_string(SHELL_NAME,false);
		read_result = read_input(read_buffer, input_buffer_size);
		if (read_result > 0) {
			parse(read_buffer, tasks, &current_task_count);
			link_tasks(tasks, current_task_count);
			if (kstrcmp(read_buffer,EXIT_COMMAND) == true) {
				is_shell_running = false;
			} else {
				task_run_result = run_task_tree(tasks, current_task_count);
				if(task_run_result == -1) {
					print_error("Error running command.");
				}
			}
		} else {
			print_error("Could not execute command, too short.");
		}
		current_task_count = 0;
	}
	print_string(EXIT_MESSAGE, true);

	return EXIT_SUCCESS;
}
