#ifndef PROCESS_H
#define PROCESS_H

#include "defs.h"
#include "task.h"

// Function declarations

/**
 * @brief Runs a single process with input/output redirection and optional waiting.
 *
 * @param process_name Name of the process to run (e.g., "/bin/ls").
 * @param arguments Null-terminated array of arguments for the process.
 * @param newenvp Environment variables for the new process.
 * @param wait_for_finish If true, the function waits for the process to complete.
 * @param input_fd File descriptor for input redirection.
 * @param output_fd File descriptor for output redirection.
 * @return The process exit status if wait_for_finish is true, or the child PID if false.
 *         Returns -1 on error.
 */
int run_process(char *process_name, char *arguments[], char **newenvp, 
                bool wait_for_finish, int input_fd, int output_fd);

/**
 * @brief Sets up input and output redirection for the current process.
 *
 * @param input_fd File descriptor for input.
 * @param output_fd File descriptor for output.
 * @return 0 on success, -1 on failure.
 */
int setup_redirection(int input_fd, int output_fd);

/**
 * @brief Executes a pipeline of tasks, linking processes together with pipes.
 *
 * @param task Pointer to the head of the task pipeline.
 * @return 0 on success, -1 on failure.
 */
int run_task_pipeline(Task *task);

/**
 * @brief Resets invalid file descriptors to standard input and output.
 *
 * @param tasks Array of task pipelines.
 * @param task_count Number of task pipelines.
 */
void clean(Task *tasks[], int task_count);

/**
 * @brief Runs multiple task pipelines concurrently.
 *
 * @param tasks Array of task pipelines.
 * @param task_count Number of task pipelines.
 * @return 0 on success, -1 on failure.
 */
int run_task_tree(Task *tasks[], int task_count);

#endif  // PROCESS_H
