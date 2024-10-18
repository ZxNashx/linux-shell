#ifndef PROCESS_H
#define PROCESS_H

#include "defs.h"
#include "task.h"

// Function declarations

/**
 * @brief Executes a pipeline of tasks, linking processes together with pipes.
 *
 * @param task Pointer to the head of the task pipeline.
 * @return 0 on success, -1 on failure.
 */
int run_task_pipeline(Task *task);

/**
 * @brief Runs multiple task pipelines concurrently.
 *
 * @param tasks Array of task pipelines.
 * @param task_count Number of task pipelines.
 * @return 0 on success, -1 on failure.
 */
int run_task_tree(Task tasks[], int task_count);
/**
 * @brief Configures input and output redirection for a given task.
 * 
 * This function ensures that the appropriate input and output streams are correctly set up
 * for the task, based on its redirection requirements. If the task specifies input or output
 * filenames, the function opens the corresponding files and redirects the standard input
 * or output. If the task is part of a pipeline, the file descriptors are redirected accordingly.
 * 
 * @param task Pointer to the Task structure to configure I/O redirection for.
 * 
 * @return 
 * - 0 on success.
 * - -1 on failure (e.g., unable to open files or set up redirection).
 * 
 * @details
 * - **Input Redirection**: If `task->input_filename` is not `NULL`, the function opens the file 
 *   for reading and redirects it to the task’s standard input (STDIN).
 * - **Output Redirection**: If `task->output_filename` is not `NULL`, the function opens the file 
 *   for writing (or creates it) and redirects it to the task’s standard output (STDOUT).
 * - **Pipes**: If the task is part of a pipeline, the function ensures that:
 *   - The previous task's output is connected to the current task's input.
 *   - The current task’s output is prepared to connect to the next task's input.
 * - **Error Handling**: If any system call fails (e.g., `open()` or `dup2()`), the function 
 *   prints an error message and returns -1 to indicate failure.
 * 
 * Example:
 * @code
 * Task task = {
 *     .args = {"/bin/cat", "input.txt", NULL},
 *     .input_filename = "input.txt",
 *     .output_filename = "output.txt",
 *     .is_pipe = false,
 *     .input_fd = -1,
 *     .output_fd = -1,
 * };
 * if (setup_io(&task) == -1) {
 *     perror("Failed to set up I/O");
 *     return -1;
 * }
 * @endcode
 */
int setup_io(Task *task);
#endif  // PROCESS_H
