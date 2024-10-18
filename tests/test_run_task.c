
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
    // Task 1: ls -al > out2.txt &
    Task task1;
    Task task2;
    Task task3;
    Task task4;

    // Initialize task1
    task1.args[0] = "/bin/ls";
    task1.args[1] = "-al";
    task1.args[2] = NULL;
    task1.arg_count = 2;
    task1.input_filename = NULL;
    task1.output_filename = "out2.txt";  // Redirect output to out2.txt.
    task1.is_background = true;          // Run in background.
    task1.is_pipe = false;               // Not part of a pipeline.
    task1.next = NULL;
    task1.prev = NULL;
    task1.input_fd = -1;
    task1.output_fd = -1;

    // Initialize task2: ls &
    task2.args[0] = "/bin/ls";
    task2.args[1] = NULL;
    task2.arg_count = 1;
    task2.input_filename = NULL;
    task2.output_filename = NULL;  // No output redirection.
    task2.is_background = true;    // Run in background.
    task2.is_pipe = false;         // Not part of a pipeline.
    task2.next = NULL;
    task2.prev = NULL;
    task2.input_fd = -1;
    task2.output_fd = -1;

    // Initialize task3 and task4: pwd | wc
    task3.args[0] = "/bin/pwd";
    task3.args[1] = NULL;
    task3.arg_count = 1;
    task3.input_filename = NULL;
    task3.output_filename = NULL;  // No output redirection.
    task3.is_background = false;   // Foreground task.
    task3.is_pipe = true;          // Part of a pipeline.
    task3.next = &task4;           // Link to the next task in the pipeline.
    task3.prev = NULL;
    task3.input_fd = -1;
    task3.output_fd = -1;

    task4.args[0] = "/usr/bin/wc";
    task4.args[1] = NULL;
    task4.arg_count = 1;
    task4.input_filename = NULL;
    task4.output_filename = NULL;  // No output redirection.
    task4.is_background = false;   // Foreground task.
    task4.is_pipe = false;         // Last task in the pipeline.
    task4.next = NULL;
    task4.prev = &task3;
    task4.input_fd = -1;
    task4.output_fd = -1;

    // Create an array of pointers to tasks
    Task *tasks[3] = {&task1, &task2, &task3};

    // Run the task tree
    run_task_tree(tasks, 3);

    return EXIT_SUCCESS;
}