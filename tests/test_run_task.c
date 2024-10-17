#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> 
#include <stdbool.h>  // For true/false
#include "process.h"
#include "defs.h"
#include "task.h"

// Function to initialize a single task
void init_task(Task *task, char *cmd, char *arg1, char *arg2, int output_fd) {
    task->arg_count = 0;
    task->args[task->arg_count++] = cmd;
    if (arg1) task->args[task->arg_count++] = arg1;
    if (arg2) task->args[task->arg_count++] = arg2;
    task->args[task->arg_count] = NULL;  // Null-terminate the argument list
    task->input_fd = STDIN_FILENO;       // Default to standard input
    task->output_fd = output_fd;         // Use provided output file descriptor or STDOUT
    task->is_background = 0;
    task->is_pipe = 0;
    task->next = NULL;
    task->prev = NULL;
}

// Function to link two tasks in a pipeline
void link_tasks(Task *first, Task *second) {
    first->next = second;
    second->prev = first;
}

// Function to create and set up tasks and pipelines
void create_tasks(Task *commands[], Task **tasks[]) {
    // Create the output file for redirection in the first pipeline
    int output_fd = open("out.test", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("Failed to open output file");
        return;
    }

    // Pipeline 1: ls -al | wc > out.test (in background)
    init_task(commands[0], "/bin/ls", "-al", NULL, -1);
    init_task(commands[1], "/bin/wc", NULL, NULL, output_fd);
    link_tasks(commands[0], commands[1]);
    commands[0]->is_pipe = true;
    commands[1]->is_pipe = true;
    commands[0]->is_background = true;
    commands[1]->is_background = true;
    commands[1]->output_fd = output_fd;
    tasks[0] = &commands[0];  // Store head of pipeline 1

    // Task 2: ls (no pipeline, background)
    init_task(commands[2], "/bin/ls", NULL, NULL, -1);
    commands[2]->is_background = true;
    commands[2]->output_fd = STDOUT_FILENO;
    tasks[1] = &commands[2];  // Store this as a single task

    // Pipeline 2: pwd | wc
    init_task(commands[3], "/bin/pwd", NULL, NULL, -1);
    init_task(commands[4], "/bin/wc", NULL, NULL, -1);
    link_tasks(commands[3], commands[4]);
    commands[3]->is_pipe = true;
    commands[4]->is_pipe = true;
    tasks[2] = &commands[3];  // Store head of pipeline 2
}

int main() {
    // Array of individual Task objects (for all commands)
    Task command_array[MAX_TASKS];  // We need 5 commands in total
    Task *commands[5] = {
        &command_array[0], &command_array[1], &command_array[2], 
        &command_array[3], &command_array[4]
    };

    // Array of task pipelines (heads of linked lists)
    int task_count = 3;
    Task **tasks[task_count];

    // Create tasks and pipelines with hardcoded commands
    create_tasks(commands, tasks);

    // Output to verify the pipelines
    printf("Pipeline 1: %s %s | %s (output to 'out.test', background)\n",
           (*tasks[0])->args[0], (*tasks[0])->args[1], 
           (*tasks[0])->next->args[0]);

    printf("Task 2: %s (background)\n", (*tasks[1])->args[0]);

    printf("Pipeline 2: %s | %s\n",
           (*tasks[2])->args[0], (*tasks[2])->next->args[0]);

    printf("Running tasks...\n");

    // Run the tasks
    int task_result = run_task_tree(*tasks, task_count);

    printf("Task result: %i\n", task_result);

    return 0;
}
