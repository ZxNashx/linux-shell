
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include "task.h"
#include "defs.h"
#include "process.h"

int main(){
    // Task 1: /bin/cat input.txt
    Task task1;
    Task task2;
    Task task3;
    task1.args[0] = "/bin/cat";
    task1.args[1] = NULL;
    task1.arg_count = 1;
    task1.input_filename = "input.txt";
    task1.output_filename = NULL;
    task1.is_background = false;
    task1.is_pipe = true;
    task1.next = &task2;
    task1.prev = NULL;
    task1.input_fd = -1;
    task1.output_fd = -1;

    // Task 2: /usr/bin/grep "search_term"
    
    task2.args[0] = "/usr/bin/grep";
    task2.args[1] = "search_term";
    task2.args[2] = NULL;
    task2.arg_count = 2;
    task2.input_filename = NULL;
    task2.output_filename = NULL;
    task2.is_background = false;
    task2.is_pipe = true;
    task2.next = &task3;
    task2.prev = &task1;
    task2.input_fd = -1;
    task2.output_fd = -1;

    // Task 3: /usr/bin/sort > output.txt
    
    task3.args[0] = "/usr/bin/sort";
    task3.args[1] = NULL;
    task3.arg_count = 1;
    task3.input_filename = NULL;
    task3.output_filename = "output.txt";
    task3.is_background = false;
    task3.is_pipe = false;
    task3.next = NULL;
    task3.prev = &task2;
    task3.input_fd = -1;
    task3.output_fd = -1;

    // Run the pipeline
    run_task_pipeline(task1);

}

