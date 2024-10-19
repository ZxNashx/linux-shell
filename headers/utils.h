#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>  // For ssize_t and file descriptor functions
#include <fcntl.h>   // For open()
#include <stdbool.h> // For bool type


/**
 * @brief Retrieves the value of an environment variable.
 *
 * This function searches for the specified environment variable in `/proc/self/environ`
 * and copies its value into the provided buffer.
 *
 * @param var_name The name of the environment variable to search for.
 * @param env_value A buffer to store the value of the environment variable.
 * @return The length of the environment variable's value on success,
 *         EXIT_FAILURE if the environment variable is not found or an error occurs.
 */
int get_env_value(char *var_name, char *env_value);

/**
 * @brief Resolves a command path using the PATH environment variable.
 *
 * This function checks if a command exists in the PATH environment variable.
 * If the command is found, the full path is constructed and stored in the new_path buffer.
 * If the command contains a '/', the original path is retained without changes.
 *
 * @param old_path The original command or path provided.
 * @param new_path A buffer to store the resolved full path if a change is made.
 * @return 1 if the path was changed (command found in PATH),
 *         EXIT_SUCCESS if no change was made (old_path contains '/'),
 *         EXIT_FAILURE if an error occurred (e.g., PATH not accessible).
 */
int check_env_path(char *old_path, char *new_path);

#endif // UTILS_H
