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

int main () {
  char *shell_name = "$= ";
	char *exit_command = "exit";

    int input_buffer_size = MAX_STR_SIZE;
    char read_buffer[input_buffer_size];
    int read_result;
    int is_shell_running = true;
    Task* test_task;
    char *env[] = {NULL};
    

    char * cmd_tokens[MAX_CMD_TOKENS];
    int token_count = 0;
    char to_split = ' ';
	bool is_shell_cmd;

    while(is_shell_running){
      int mycount = 0;
      print_string(shell_name,false);
      read_result = read_input(read_buffer, input_buffer_size);
      print_string("Entered: ",false);
      print_string(read_buffer,true);

      get_tasks(test_task, read_buffer, cmd_tokens, &token_count, to_split);


      is_shell_cmd = false;
      printf("%i\n",test_task->arg_count);
      while (mycount < test_task->arg_count) {
        printf("arg: %s\n", test_task->args[mycount++]);
      }
      if(kstrcmp(read_buffer,exit_command) == true){
        is_shell_cmd = true;
        is_shell_running = false;
      } else {
        run_process(test_task->args[0], cmd_tokens, env, 
        true, task->input_file, task->output_file);
      }
    }

    return 0;


  return 0;
}