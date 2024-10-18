#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

// Include the necessary standard headers for signals
#include <signal.h>

/**
 * @brief Signal handler for SIGINT (Interrupt Signal).
 *
 * This function is invoked when the process receives the SIGINT signal, typically sent
 * by pressing `Ctrl+C` in the terminal. It allows the process to handle the interrupt
 * gracefully by performing necessary cleanup and terminating the process.
 *
 * @param sig The signal number that triggered the handler. For SIGINT, this value will be
 *            the constant `SIGINT`.
 *
 * @details
 * This handler function ensures that when the user interrupts the process (e.g., with 
 * `Ctrl+C`), the process has an opportunity to clean up resources, close files, or 
 * perform any other necessary operations before exiting. The use of `exit(0)` ensures
 * a clean termination with an exit code of 0, indicating normal termination.
 *
 * Example usage:
 * @code
 * struct sigaction sa;
 * sa.sa_handler = handle_sigint;
 * sigemptyset(&sa.sa_mask);
 * sa.sa_flags = 0;
 * sigaction(SIGINT, &sa, NULL);  // Register the handler
 * @endcode
 *
 * @note
 * - This handler only handles SIGINT; other signals require their own handlers.
 * - If the process has open files or other allocated resources, this is the place to 
 *   release them.
 *
 * @see sigaction, exit
 */
void handle_sigint(int sig);

#endif  // SIGNAL_HANDLER_H
