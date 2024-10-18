#include "defs.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdint.h>
#include "task.h"
#include "parser.h"
#include "str.h"
/*
static uint8_t memory_pool[MEMORY_POOL_SIZE];
static size_t memory_offset = 0;  

static Task *task_pool[MAX_array];
static int task_pool_index = 0;  

// Function to link two array in a pipeline
void link_array(Task *first, Task *second) {
    first->next = second;
    second->prev = first;
    first->is_pipe = true;
    second->is_pipe = true;
}
*/

//hello kevin
//hello axyl
void get_tasks(Task_List job_list, char *str_to_split, char *tokens[],
               int *count, char *temp_string) {

    char *temp_tokens[MAX_CMD_TOKENS]; //a temporary array to split args to different array
    int temp_token_count = 0;          //number of tokens in temp_tokens
    // Split the input string into tokens based on spaces or other delimiters
    //int split_str_result = split_str(str_to_split, tokens, count, SPACE, original);  // Assuming split_str() is implemented
    int split_str_result;
    
    
    //int curr_token = 0;
    //int arg_count = 0;

    //Firstly separate using '&' and fill the Task_List
    if(kstrhas_unary(str_to_split, RUN_IN_BKG)) {
        split_str_result = split_str(str_to_split, temp_tokens, &temp_token_count, RUN_IN_BKG, temp_string);
        //tokenize(str_to_split, tokens, &temp_token_count, SPACE) **REMOVE IF tokenize NOT IMPLEMENTED**
    }

    if (split_str_result == -1) { //if split_str() failed to execute
        return; //probably add some kind of error catching instead here later
    }

    //tokens now holds the '&' separated strings
    for (int i = 0; i < temp_token_count; i++) {
        job_list.list[i]->args[i] = temp_tokens[i];
        if ((i + 1) < temp_token_count) {
            job_list.list[i]->is_background = true;
        } else {
            job_list.list[i]->is_background = false;
        }
        
        tokens[i] = temp_tokens[i];
    }
/*
    //Separate strings by '|'
    for (int i = 0; i < temp_token_count; i++) {
        if (kstrhas_unary(tokens[i], PIPE)) {
            split_str_result = split_str(tokens[i], temp_tokens, &temp_token_count, PIPE, temp_string);
        }
    }

    if (split_str_result == -1) { //if split_str() failed to execute
        return; //probably add some kind of error catching instead here later
    }

    //transferring '|' separated strings to tokens
    for (int i = 0; i < temp_token_count; i++) {
        tokens[i] = temp_tokens[i];
    }
*/
    
    


    //Secondly separate pipes


    // Initialize task fields (if needed)

   /* Task *task;

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
    
    */
}

void init_task_list(Task_List tasks, Task array[]) {
    for (int i = 0; i < MAX_NUM_TASKS; i++) {
        array[i].args[0] = NULL;
        array[i].arg_count = 0;
        array[i].input_fd = STDIN_FILENO;
        array[i].output_fd = STDOUT_FILENO;
        array[i].input_filename = NULL;
        array[i].output_filename = NULL;
        array[i].is_background = false;
        array[i].next = NULL;
        array[i].prev = NULL;
        tasks.list[i] = &array[i];
    }
}


/*
void task_memory_init() {
    memory_offset = 0;  
    task_pool_index = 0;
    for (int i = 0; i < MAX_array; i++) {
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
    if (task_pool_index >= MAX_array) {
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
*/
