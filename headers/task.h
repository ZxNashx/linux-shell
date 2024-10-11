#ifndef TASK_H
#define TASK_H

#include "defs.h"

// Task structure definition
typedef struct Task {
    char *args[MAX_CMD_TOKENS];  // Command and arguments
    char *input_file;            // Input redirection file
    char *output_file;           // Output redirection file
    int is_background;           // Background task flag
    struct Task *next;           // Pointer to the next task in a pipeline
} Task;

// Function Prototypes
void task_memory_init();  // Initializes the memory pool for Task allocations
Task *task_alloc();       // Allocates a new Task from the memory pool
void task_free(Task *task);  // Frees a Task back to the memory pool
void display_task_memory();  // Debug function to display memory pool status

#endif  // TASK_H
