#ifndef TASK_H
#define TASK_H

#include "defs.h"



/**
 * @brief Structure to represent a task (or command) in a pipeline for a shell program.
 * 
 * This structure holds all necessary information to manage the execution of a single task or 
 * command. It includes arguments, input/output file descriptors, filenames for redirection, 
 * and flags for background execution and pipelines. Each task can be linked to others to 
 * form a pipeline using `next` and `prev` pointers.
 */
typedef struct Task {
    char *args[MAX_CMD_TOKENS];  /**< Array of command arguments, including the command itself.
                                      This must be null-terminated to work with exec functions. */

    int arg_count;               /**< Number of arguments provided, including the command name. */

    int input_fd;                /**< Input file descriptor.
                                      - Holds the file descriptor for redirected input.
                                      - Default: -1 (not set).
                                      - If no redirection: STDIN_FILENO (0). */

    int output_fd;               /**< Output file descriptor.
                                      - Holds the file descriptor for redirected output.
                                      - Default: -1 (not set).
                                      - If no redirection: STDOUT_FILENO (1). */

    char *input_filename;        /**< Name of the file used for input redirection (if any).
                                      - Used to open input_fd if provided. */

    char *output_filename;       /**< Name of the file used for output redirection (if any).
                                      - Used to open output_fd if provided. */

    int is_background;           /**< Flag to indicate if the task should run in the background.
                                      - true: Run in background.
                                      - false: Run in foreground (default). */

    int is_pipe;                 /**< Flag to indicate if the task is part of a pipeline.
                                      - true: Task is part of a pipeline.
                                      - false: Not part of a pipeline (default). */

    struct Task *next;           /**< Pointer to the next task in the pipeline (if any).
                                      - NULL if this is the last task in the pipeline. */

    struct Task *prev;           /**< Pointer to the previous task in the pipeline (if any).
                                      - NULL if this is the first task in the pipeline. */
    int prev_pipe_fd[2];    /**< File descriptors for the pipe connected to the previous task.
                                - prev_pipe_fd[0]: Read end of the pipe.
                                - prev_pipe_fd[1]: Write end of the pipe.
                                - Both are set to -1 if there is no pipe from the previous task. */
} Task;


//unused, probably get rid of
typedef struct Task_List {
    Task *list[MAX_NUM_TASKS]; //an array of tasks from which we can run processes
} Task_List;

//unused, probably get rid of
void init_task_list(Task_List *tasks, Task array[]);
void get_tasks(Task_List *job_list, char *str_to_split, char *tokens[],
               int *count, char *temp_string, char *temp_string2);

// Function Prototypes
void task_memory_init();  // Initializes the memory pool for Task allocations
Task *task_alloc();       // Allocates a new Task from the memory pool
void task_free(Task *task);  // Frees a Task back to the memory pool
void display_task_memory();  // Debug function to display memory pool status

#endif  // TASK_H
