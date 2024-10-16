
#include <unistd.h>    // For fork(), execve(), dup2(), _exit()
#include <sys/wait.h>  // For waitpid()
#include <fcntl.h>     // For open() flags, if needed
#include <stdio.h>     // For perror()

#include "defs.h"

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


