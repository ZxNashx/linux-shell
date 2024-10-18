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


// Corrected setup_io function
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

    // Initialize previous pipe file descriptors
    prev_pipe_fd[0] = -1;
    prev_pipe_fd[1] = -1;

    while (task != NULL) {
        // Set up input/output redirection for the task
        if (setup_io(task) == -1) {
            // Error occurred during setup_io
            // Cleanup and exit
            if (prev_pipe_fd[0] != -1) close(prev_pipe_fd[0]);
            if (prev_pipe_fd[1] != -1) close(prev_pipe_fd[1]);
            _exit(EXIT_FAILURE);  // Use _exit to exit the child process immediately
        }

        // Set up pipes if needed
        if (task->output_fd == -1 && task->next != NULL) {
            // Create a new pipe
            if (pipe(pipe_fd) == -1) {
                // Handle error using write
                write(STDERR_FILENO, "Error: pipe failed\n", 19);
                // Close any open file descriptors
                if (prev_pipe_fd[0] != -1) close(prev_pipe_fd[0]);
                if (prev_pipe_fd[1] != -1) close(prev_pipe_fd[1]);
                _exit(EXIT_FAILURE);  // Use _exit to exit the child process immediately
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
            if (prev_pipe_fd[0] != -1) close(prev_pipe_fd[0]);
            if (prev_pipe_fd[1] != -1) close(prev_pipe_fd[1]);
            if (pipe_fd[0] != -1) close(pipe_fd[0]);
            if (pipe_fd[1] != -1) close(pipe_fd[1]);
            _exit(EXIT_FAILURE);  // Use _exit to exit the child process immediately
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
            if (prev_pipe_fd[0] != -1) close(prev_pipe_fd[0]);
            if (prev_pipe_fd[1] != -1) close(prev_pipe_fd[1]);
            if (pipe_fd[0] != -1) close(pipe_fd[0]);
            if (pipe_fd[1] != -1) close(pipe_fd[1]);

            // Execute the command
            // Since we are restricted to using execve and only low-level system calls,
            // we will assume that args[0] contains the full path to the executable.

            // Prepare environment variables
            extern char **environ;
            execve(task->args[0], task->args, environ);

            // If execve returns, an error occurred
            write(STDERR_FILENO, "Error: execve failed\n", 21);
            _exit(EXIT_FAILURE);
        } else {
            // Parent process

            // Close unused file descriptors
            if (prev_pipe_fd[0] != -1) close(prev_pipe_fd[0]);
            if (prev_pipe_fd[1] != -1) close(prev_pipe_fd[1]);
            if (task->input_fd != -1 && task->input_fd != STDIN_FILENO) close(task->input_fd);
            if (task->output_fd != -1 && task->output_fd != STDOUT_FILENO) close(task->output_fd);
            if (pipe_fd[1] != -1) close(pipe_fd[1]); // Close write end in parent

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

int run_task_tree(Task *tasks[], int task_count) {
    pid_t pids[task_count];
    int status;

    for (int i = 0; i < task_count; ++i) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork failed");
            return -1;
        }
        if (pids[i] == 0) {  // Child process
            run_task_pipeline(tasks[i]);
            _exit(EXIT_SUCCESS);  // Ensure child process exits after running the pipeline
        }
    }

    // Parent process waits for all child processes
    for (int i = 0; i < task_count; ++i) {
        if (waitpid(pids[i], &status, 0) == -1) {
            perror("waitpid failed");
        }
    }

    return 0;
}

/*
#include <unistd.h>    // For fork(), execve(), dup2(), _exit()
#include <sys/wait.h>  // For waitpid()
#include <fcntl.h>     // For open() flags, if needed
#include <stdio.h>     // For perror()

#include "defs.h"
#include "task.h"

#define MAX_LISTS 10  // Maximum number of process lists in the process tree

int run_process(char *process_name, char *arguments[], char **newenvp, bool wait_for_finish, int input_fd, int output_fd) {
    pid_t pid;
    int status;

    pid = fork();

    if (pid == -1) {
        // Failed to fork
        perror("fork failed");
        return -1;
    }

    if (pid == 0) {
        // This is the child process
        
        // Redirect input if input_fd is not STDIN_FILENO
        if (input_fd != STDIN_FILENO) {
            if (dup2(input_fd, STDIN_FILENO) == -1) {
                perror("dup2 input_fd failed");
                _exit(1); // Exit with error code
            }
            close(input_fd);  // Close the old input_fd
        }

        // Redirect output if output_fd is not STDOUT_FILENO
        if (output_fd != STDOUT_FILENO) {
            if (dup2(output_fd, STDOUT_FILENO) == -1) {
                perror("dup2 output_fd failed");
                _exit(1); // Exit with error code
            }
            close(output_fd); // Close the old output_fd
        }

        // Execute the new process
        execve(process_name, arguments, newenvp);
        // If execve fails, terminate the child process
        perror("execve failed");
        _exit(1);
    } else {
        // This is the parent process
        if (wait_for_finish) {
            // Wait for the child to finish
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid failed");
                return -1;
            }
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);  // Return child's exit status
            } else {
                return -1;  // Process did not exit normally
            }
        } else {
            // Return the child PID if not waiting
            return pid;
        }
    }
}


// Function to setup input/output redirection in child processes
int setup_redirection(int input_fd, int output_fd) {
    if (input_fd != STDIN_FILENO) {
        if (dup2(input_fd, STDIN_FILENO) == -1) {
            perror("dup2 (input redirection) failed");
            return -1;
        }
        close(input_fd);
    }
    if (output_fd != STDOUT_FILENO) {
        if (dup2(output_fd, STDOUT_FILENO) == -1) {
            perror("dup2 (output redirection) failed");
            return -1;
        }
        close(output_fd);
    }
    return 0;
}

// Function to execute a pipeline of tasks
int run_task_pipeline(Task *task) {
    int pipefd[2];  // Pipe file descriptors
    int input_fd = task->input_fd;  // Use task's input file descriptor or STDIN
    int output_fd;

    while (task != NULL) {
        // Setup output redirection or create a pipe
        if (task->is_pipe && task->next) {
            if (pipe(pipefd) == -1) {
                perror("pipe failed");
                return -1;
            }
            output_fd = pipefd[1];  // Write end of the pipe
        } else {
            output_fd = task->output_fd != -1 ? task->output_fd : STDOUT_FILENO;
        }

        // Fork a new process
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            return -1;
        } else if (pid == 0) {  // Child process
            // Set up input and output redirection
            if (setup_redirection(input_fd, output_fd) == -1) {
                exit(EXIT_FAILURE);
            }
            // Execute the command
            execvp(task->args[0], task->args);
            perror(task->args[0]);
            perror("execvp failed");  // If execvp fails, exit
            exit(EXIT_FAILURE);
        }

        // Parent process: close used descriptors
        if (input_fd != STDIN_FILENO) close(input_fd);
        if (output_fd != STDOUT_FILENO) close(output_fd);

        // Move to the next task in the pipeline
        input_fd = pipefd[0];  // Read end of the pipe becomes the next input
        task = task->next;
    }

    // Wait for all child processes to finish if not background task
    if (!task || !task->is_background) {
        int status;
        while (wait(&status) > 0);  // Wait for all child processes
    }

    return 0;
}


void set_fd(Task *task) {
    // Set default file descriptors to standard input/output
    task->input_fd = STDIN_FILENO;
    task->output_fd = STDOUT_FILENO;

    // Open input file if specified
    if (task->input_filename != NULL) {
        task->input_fd = open(task->input_filename, O_RDONLY);
        if (task->input_fd == -1) {
            perror("Failed to open input file");
            task->input_fd = STDIN_FILENO;  // Fall back to standard input
        }
    }

    // Open output file if specified
    if (task->output_filename != NULL) {
        task->output_fd = open(task->output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (task->output_fd == -1) {
            perror("Failed to open output file");
            task->output_fd = STDOUT_FILENO;  // Fall back to standard output
        }
    }
}

// Function to run multiple task pipelines concurrently
int run_task_tree(Task *tasks[], int task_count) {

    pid_t pids[MAX_LISTS];

    for (int i = 0; i < task_count; ++i) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork failed");
            return -1;
        }
        if (pids[i] == 0) {  // Child process
            if (run_task_pipeline(tasks[i]) == -1) {
                perror("run_task_pipeline failed");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process: wait for non-background tasks
    for (int i = 0; i < task_count; ++i) {
        if (!tasks[i]->is_background) {
            if (waitpid(pids[i], NULL, 0) == -1) {
                perror("waitpid failed");
                return -1;
            }
        }
    }

    return 0;
}
*/