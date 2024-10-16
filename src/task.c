#include "defs.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "task.h"

static uint8_t memory_pool[MEMORY_POOL_SIZE];
static size_t memory_offset = 0;  

static Task *task_pool[MAX_TASKS];
static int task_pool_index = 0;  
//hello kevin
//hello axyl
void get_tasks(Task *task, char *str_to_split, char *tokens[], int *count, char split_on) {
    // Split the input string into tokens based on spaces or other delimiters
    char *temp = str_to_split;
    int split_str_result = split_str(temp, tokens, count, split_on);  // Assuming split_str() is implemented
    int curr_token = 0;
    int arg_count = 0;

    // Initialize task fields (if needed)
    task->arg_count = 0;
    task->input_file = NULL;//standard input
    task->output_file = NULL;//standard output
    task->is_background = 0;  // 0: No pipe, 1: Piped

    // Process tokens
    while (arg_count < *count) {
        if (kstrcmp(tokens[curr_token], "<") == true) {
            // Input redirection
            curr_token++;
            if (tokens[curr_token] != NULL) {
                task->input_file = tokens[curr_token];  // Store the input file name
            }
        } else if (kstrcmp(tokens[curr_token], ">") == true) {
            // Output redirection
            task->next->output_file = tokens[curr_token++];
            if (tokens[curr_token] != NULL) {
                task->output_file = tokens[curr_token];  // Store the output file name
            }
        } else if (kstrcmp(tokens[curr_token], "|") == true) {
            // Pipe handling
            task->is_pipe = 1;  // Indicate that there is a pipe
            break;  // Stop processing for this task, as the rest is part of the next task
        } else {
            // Regular argument
            task->args[arg_count] = tokens[curr_token];  // Store the argument
            arg_count++;
        }
        curr_token++;
    }
    task->arg_count = arg_count;
    // Null-terminate the arguments list
    task->args[arg_count] = NULL;
}




void task_memory_init() {
    memory_offset = 0;  
    task_pool_index = 0;
    for (int i = 0; i < MAX_TASKS; i++) {
        task_pool[i] = NULL;  
    }
}


void *alloc_memory(size_t size) {
    if (memory_offset + size > MEMORY_POOL_SIZE) {
        return NULL; 
    }
    void *allocated_memory = &memory_pool[memory_offset];
    memory_offset += size;
    return allocated_memory;
}

Task *task_alloc() {
    if (task_pool_index >= MAX_TASKS) {
        printf("Error: Maximum task allocation reached.\n");
        return NULL;
    }

    Task *new_task = (Task *)alloc_memory(sizeof(Task));
    if (!new_task) {
        printf("Error: Not enough memory in the pool for a new Task.\n");
        return NULL;
    }

    for (int i = 0; i < MAX_CMD_TOKENS; i++) {
        new_task->args[i] = NULL;
    }
    new_task->input_file = NULL;
    new_task->output_file = NULL;
    new_task->is_background = 0;
    new_task->next = NULL;

    task_pool[task_pool_index++] = new_task;
    return new_task;
}

void task_free(Task *task) {
    if (task == NULL) {
        return;
    }
    for (int i = 0; i < task_pool_index; i++) {
        if (task_pool[i] == task) {
            for (int j = i; j < task_pool_index - 1; j++) {
                task_pool[j] = task_pool[j + 1];
            }
            task_pool[--task_pool_index] = NULL;
            break;
        }
    }
}

void display_task_memory() {
    printf("Memory Pool Usage: %zu / %d bytes used.\n", memory_offset, MEMORY_POOL_SIZE);
    printf("Allocated Task structures: %d\n", task_pool_index);
    for (int i = 0; i < task_pool_index; i++) {
        printf("  Task %d at %p\n", i, (void *)task_pool[i]);
    }
}
