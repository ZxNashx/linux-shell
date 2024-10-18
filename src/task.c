#include "defs.h"
#include "task.h"
#include "parser.h"
#include "str.h"
#include <fcntl.h>
#include <unistd.h>
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
void get_tasks(Task_List *job_list, char *str_to_split, char *tokens[],
               int *count, char *temp_string, char *temp_string2) {
    bool background_status[MAX_NUM_TASKS];
    char *temp_tokens[MAX_CMD_TOKENS]; //a temporary array to split args to different array
    int temp_token_count = 0;          //number of tokens in temp_tokens
    int temp_job_arg_count = 0;        //number of args in a Task
    
    // Split the input string into tokens based on spaces or other delimiters
    //int split_str_result = split_str(str_to_split, tokens, count, SPACE, original);  // Assuming split_str() is implemented
    int split_str_result;
    

    //int curr_token = 0;
    int skip_count = 0;     //used to reinsert array elements in the right spot

    //Firstly separate using '&' and fill the Task_List
    if(kstrhas_unary(str_to_split, RUN_IN_BKG)) {
        split_str_result = split_str(str_to_split, temp_tokens, &temp_token_count, RUN_IN_BKG, temp_string);
        //tokenize(str_to_split, tokens, &temp_token_count, SPACE) **REMOVE IF tokenize NOT IMPLEMENTED**
    }

    if (split_str_result == -1) { //if split_str() failed to execute
        return; //probably add some kind of error catching instead here later
    }
    int i;
    //temp_tokens now hold the '&' separated strings
    for (i = 0; i < temp_token_count; i++) {
        job_list->list[i]->args[0] = temp_tokens[i];
        if ((i + 1) < temp_token_count) {
            job_list->list[i]->is_background = true;
        } else {
            job_list->list[i]->is_background = false;
        }
        tokens[i] = temp_tokens[i];
        job_list->list[i]->arg_count++;
        temp_job_arg_count++;
    }

    //holds the current background/foreground task status
    for (i = 0; i < temp_job_arg_count; i++) {
        background_status[i] = job_list->list[i]->is_background;
    }
    

    //Separate strings by '|'
    for (int i = 0; i < temp_job_arg_count; i++) {
        if (kstrhas_unary(tokens[i], PIPE)) {
            split_str_result = split_str(tokens[i], temp_tokens, &temp_token_count, PIPE, temp_string2);
        } else {
            //in case there are no pipes we still want to copy the old strings
            kstrcpy(tokens[i], temp_tokens[0]);
            temp_token_count = 1; //resetting in case of no pipes
        }

        if (split_str_result == -1) { //if split_str() failed to execute
            return; //probably add some kind of error catching instead here later
        }

        for (int j = 0; j < temp_token_count; j++) {
            if(background_status[i] == true) {
                job_list->list[j]->is_background = true;
            } else {
                job_list->list[j + skip_count]->is_background = false;
            }
            if (i >= 1) {
                job_list->list[j + skip_count]->args[0] = temp_tokens[j];
            } else {
                job_list->list[j]->args[0] = temp_tokens[j];
            }
            skip_count++;

            if ((j + 1) < temp_token_count) {
                job_list->list[j]->next = job_list->list[j + 1];
            } else {
                job_list->list[j]->prev = job_list->list[j - 1];
            }
            job_list->list[j]->is_pipe = true;
        }
    }

    //holds the current background/foreground task status
    for (i = 0; i < temp_job_arg_count; i++) {
        background_status[i] = job_list->list[i]->is_background;
    }
    //transferring '|' separated strings to tokens
    for (int i = 0; i < temp_token_count; i++) {
        tokens[i] = temp_tokens[i];
    }
}
/*
  Input: Task_List *tasks - list of all tasks to be run
         Task array [][] - allocated memory space for list of all jobs and tasks

  Returns: Nothing yet

*/
void init_task_list(Task_List *tasks, Task array[]) {
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
        tasks->list[i] = &array[i];
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
                task_poo] = task_pool[j + 1];
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
