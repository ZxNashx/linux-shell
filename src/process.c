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

/**DUP2(oldfd, newfd)
 * 
How It Works
    If oldfd is a valid open file descriptor, and newfd is different from oldfd:
    dup2 will copy the oldfd file descriptor into newfd.

    If newfd is already open, dup2 closes it first to free it for reuse.
    After the call, both oldfd and newfd refer to the same resource (file, pipe, etc.), with identical properties (like read/write mode).
Key Properties
    Any modifications made through one descriptor are reflected when accessed via the other.
    Both descriptors share the same file offset (for files) and status flags.
    When either oldfd or newfd is closed, the other remains valid unless explicitly closed.
 * 
 */

int setup_io(Task *task) {
    // Input Redirection
    if (task->input_filename) {
        // Open the input file
        task->input_fd = open(task->input_filename, O_RDONLY);
        if (task->input_fd < 0) {
            write(STDERR_FILENO, "Error: Cannot open input file\n", 30);
            return -1;
        }
    } else {
        if (task->prev == NULL) {
            // First task in the pipeline without input redirection
            task->input_fd = STDIN_FILENO;
        } else {
            // Intermediate task; input will come from the pipe
            task->input_fd = -1;
        }
    }

    // Output Redirection
    if (task->output_filename) {
        // Open the output file
        task->output_fd = open(task->output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (task->output_fd < 0) {
            write(STDERR_FILENO, "Error: Cannot open output file\n", 31);
            return -1;
        }
    } else {
        if (task->next == NULL) {
            // Last task in the pipeline without output redirection
            task->output_fd = STDOUT_FILENO;
        } else {
            // Intermediate task; output will go to the pipe
            task->output_fd = -1;
        }
    }

    return 0;  // Success
}

void run_task_pipeline(Task *task_head) {
    Task *task = task_head;
    int pipe_fd[2];         // For the current pipe
    int prev_pipe_fd[2];    // For the previous pipe
    pid_t pid;
    int status;

    if(task->is_pipe == true && task->prev != NULL){
        // trying to run a piped task...
        return -1;
    }

    // Initialize previous pipe file descriptors
    prev_pipe_fd[0] = -1;
    prev_pipe_fd[1] = -1;

    while (task != NULL) {
        // Set up input/output redirection for the task
        if (setup_io(task) == -1) {
            // Error occurred during setup_io
            // Cleanup and exit
            if (prev_pipe_fd[0] != -1)
                close(prev_pipe_fd[0]);
            if (prev_pipe_fd[1] != -1)
                close(prev_pipe_fd[1]);
            _exit(EXIT_FAILURE);
        }

        // Set up pipes if needed
        if (task->output_fd == -1 && task->next != NULL) {
            // Create a new pipe
            if (pipe(pipe_fd) == -1) {
                // Handle error using write
                write(STDERR_FILENO, "Error: pipe failed\n", 19);
                // Close any open file descriptors
                if (prev_pipe_fd[0] != -1)
                    close(prev_pipe_fd[0]);
                if (prev_pipe_fd[1] != -1)
                    close(prev_pipe_fd[1]);
                _exit(EXIT_FAILURE);  
            }
        } else {
            // No pipe needed
            pipe_fd[0] = -1;
            pipe_fd[1] = -1;
        }

        // Fork the process
        pid = fork();
        if (pid == -1) {
            // Handle error
            write(STDERR_FILENO, "Error: fork failed\n", 19);
            // Close any open file descriptors
            if (prev_pipe_fd[0] != -1)
                close(prev_pipe_fd[0]);
            if (prev_pipe_fd[1] != -1)
                close(prev_pipe_fd[1]);
            if (pipe_fd[0] != -1)
                close(pipe_fd[0]);
            if (pipe_fd[1] != -1)
                close(pipe_fd[1]);
            _exit(EXIT_FAILURE);  
        } else if (pid == 0) {
            // Child process

            // Input redirection
            if (task->input_fd != -1 && task->input_fd != STDIN_FILENO) {
                dup2(task->input_fd, STDIN_FILENO);
                close(task->input_fd);
            } else if (prev_pipe_fd[0] != -1) {
                // Input from previous pipe
                dup2(prev_pipe_fd[0], STDIN_FILENO);
            }
            // Else, input remains as STDIN_FILENO

            // Output redirection
            if (task->output_fd != -1 && task->output_fd != STDOUT_FILENO) {
                dup2(task->output_fd, STDOUT_FILENO);
                close(task->output_fd);
            } else if (pipe_fd[1] != -1) {
                // Output to next pipe
                dup2(pipe_fd[1], STDOUT_FILENO);
            }
            // Else, output remains as STDOUT_FILENO

            // Close unused file descriptors
            if (prev_pipe_fd[0] != -1)
                close(prev_pipe_fd[0]);
            if (prev_pipe_fd[1] != -1)
                close(prev_pipe_fd[1]);
            if (pipe_fd[0] != -1)
                close(pipe_fd[0]);
            if (pipe_fd[1] != -1)
                close(pipe_fd[1]);

            // untested ------------------
            // Before execve
            char resolved_path[256]; // Adjust size as needed
            int path_result = check_env_path(task->args[0], resolved_path);

            if (path_result == 1 || path_result == 0) {
                // Path resolved successfully, use resolved_path
                execve(resolved_path, task->args, NULL);
            } else {
                // Error occurred, command not found
                write(STDERR_FILENO, "Error: Command not found\n", 24);
                _exit(EXIT_FAILURE);
            }

            // If execve returns, an error occurred
            write(STDERR_FILENO, "Error: execve failed\n", 21);
            _exit(EXIT_FAILURE);

            write(STDERR_FILENO, "Error: execve failed\n", 21);
            _exit(EXIT_FAILURE);
            // untested ------------------
            
        } else {
            // Parent process

            // Close unused file descriptors
            if (prev_pipe_fd[0] != -1)
                close(prev_pipe_fd[0]);
            if (prev_pipe_fd[1] != -1)
                close(prev_pipe_fd[1]);
            if (task->input_fd != -1 && task->input_fd != STDIN_FILENO)
                close(task->input_fd);
            if (task->output_fd != -1 && task->output_fd != STDOUT_FILENO)
                close(task->output_fd);
            if (pipe_fd[1] != -1)
                close(pipe_fd[1]); // Close write end in parent

            // Save the read end of the current pipe for the next task
            prev_pipe_fd[0] = pipe_fd[0];
            prev_pipe_fd[1] = -1; // Write end is closed in parent

            // If the task is not running in the background, wait for it
            if (!task->is_background) {
                waitpid(pid, &status, 0);
            }

            // Move to the next task
            task = task->next;
        }
    }

    // Clean up any remaining child processes
    while ((pid = waitpid(-1, &status, 0)) > 0);
}


int run_task_tree(Task tasks[], int task_count) {
    pid_t pids[MAX_TASKS];
    int status;

    for (int i = 0; i < task_count; i++) {
        // If the task has a previous task, skip it as it will be handled as part of a pipeline
        if (tasks[i].prev != NULL) {
            continue;
        }

        pids[i] = fork();
        if (pids[i] == -1) {
            write(STDERR_FILENO, "fork failed\n", 12);
            return -1;
        }
        if (pids[i] == 0) {  // Child process
            run_task_pipeline(&tasks[i]);
            _exit(EXIT_SUCCESS);  // Ensure child process exits after running the pipeline
        }
    }

    // Parent process waits for all child processes
    for (int i = 0; i < task_count; ++i) {
        // Only wait for tasks that are not part of a pipeline
        if (tasks[i].prev != NULL) {
            continue;
        }
        if (!tasks[i].is_background) {
            if (waitpid(pids[i], &status, 0) == -1) {
                write(STDERR_FILENO, "waitpid failed\n", 15);
            }
        }
    }

    return 0;
}