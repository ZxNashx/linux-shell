#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "defs.h"

#define MAX_LISTS 10  // Maximum number of process lists in the process tree

int run_process(char *process_name, char **arguments, char **newenvp, bool wait_for_finish) {
    pid_t pid;
    int status;

    pid = fork();

    if (pid == -1) {
        // Failed to fork
        return -1;
    }

    if (pid == 0) {
        // This is the child process
        execve(process_name, arguments, newenvp);
        _exit(0); // Exit child process immediately
    } else {
        // This is the parent process
        if (wait_for_finish) {
            if (waitpid(pid, &status, 0) == -1) {
                return -1;
            }
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            } else {
                return -1; // Process exited abnormally
            }
        } else {
            // Return the child PID when not waiting
            return pid;
        }
    }
}


