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
    //basic shell inits. for main
    char *shell_name = "$= ";
	  char *exit_command = "exit";
    char read_buffer[MAX_STR_SIZE];
    int read_result;
    int is_shell_running = true;
    Task_List task_list;
    //char *env[] = {NULL};
    char * cmd_tokens[MAX_CMD_TOKENS];
    int task_count = 0;
	  bool is_shell_cmd;
    char temp_string[MAX_STR_SIZE];

    //main program loop
    while(is_shell_running){
      //int mycount = 0;
      print_string(shell_name,false);
      read_result = read_input(read_buffer, MAX_STR_SIZE); //get input from user.
      if (read_result == -1) {
        return 0; //could not read input error
      }
      get_tasks(task_list, read_buffer, cmd_tokens, &task_count, temp_string);
      printf("task count %i\n", task_count);

      is_shell_cmd = false;
      if(kstrcmp(read_buffer, exit_command) == true){
        is_shell_cmd = true;
        is_shell_running = false;
      } else if (is_shell_cmd){
        //run processess and commands
      }

      /*
      // Print out the tokens for debugging
    for (int i = 0; i < token_index; i++) {
        printf("Token[%d]: %s, length %i\n", i, tokens[i], kstrlen(tokens[i]));
    }

    */
    }

  return 0;
}