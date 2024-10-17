#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#include "defs.h"
#include "task.h"

int run_task_pipeline(Task *task, int input_fd);

// Helper function to open input and output redirection files
void handle_redirection(Task *task) {
    if (task->input_filename) {
        task->input_fd = open(task->input_filename, O_RDONLY);
        if (task->input_fd == -1) {
            perror("Failed to open input file");
            exit(EXIT_FAILURE);
        }
    }

    if (task->output_filename) {
        task->output_fd = open(task->output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (task->output_fd == -1) {
            perror("Failed to open output file");
            exit(EXIT_FAILURE);
        }
    }
}

int run_task_pipeline(Task *task, int input_fd) {
    if (task == NULL) {
        return -1;
    }

    char *dummy_envp[] = {NULL};
    int pipefd[2];  // Pipe for connecting tasks

    // If this is not the last task, create a pipe
    if (task->next != NULL && pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }

    // Fork the process to execute the current task
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {  // Child process
        // Handle input redirection
        if (task->input_fd != -1) {
            dup2(task->input_fd, STDIN_FILENO);
            close(task->input_fd);
        } else if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        // Handle output redirection
        if (task->output_fd != -1) {
            dup2(task->output_fd, STDOUT_FILENO);
            close(task->output_fd);
        } else if (task->next != NULL) {
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
        }

        close(pipefd[0]);  // Close unused read end of the pipe in child

        // Execute the command using execve
        execve(task->args[0], task->args, dummy_envp);
        perror("execve");  // If execve fails, print the error
        exit(EXIT_FAILURE);  // Exit if execve fails
    } else {  // Parent process
        if (!task->is_background) {
            // Wait for the child process to finish if not running in the background
            waitpid(pid, NULL, 0);
        }

        // Close the write end of the pipe in the parent process
        close(pipefd[1]);

        if (task->next != NULL) {
            // Recursively run the next task in the pipeline
            return run_task_pipeline(task->next, pipefd[0]);
        }

        // Close the read end if this is the last task
        close(pipefd[0]);
    }

    return 0;
}


void set_fd(Task *task) {
    task->input_fd = STDIN_FILENO;
    task->output_fd = STDOUT_FILENO;

    if (task->input_filename) {
        task->input_fd = open(task->input_filename, O_RDONLY);
        if (task->input_fd == -1) {
            perror("Failed to open input file");
            task->input_fd = STDIN_FILENO;
        }
    }

    if (task->output_filename) {
        task->output_fd = open(task->output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (task->output_fd == -1) {
            perror("Failed to open output file");
            task->output_fd = STDOUT_FILENO;
        }
    }
}

int run_task_tree(Task *tasks[], int task_count) {
    pid_t pids[MAX_LISTS];

    for (int i = 0; i < task_count; ++i) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork failed");
            return -1;
        }
        if (pids[i] == 0) {  // Child process
            set_fd(tasks[i]);
            if (run_task_pipeline(tasks[i]) == -1) {
                perror("run_task_pipeline failed");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process: Wait for non-background tasks
    for (int i = 0; i < task_count; ++i) {
        if (!tasks[i]->is_background) {
            if (waitpid(pids[i], NULL, 0) == -1 && errno != ECHILD) {
                perror("waitpid failed");
                return -1;
            }
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