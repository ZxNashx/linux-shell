#ifndef DEFS_H
#define DEFS_H
/**
 * @def EXIT_MESSAGE
 * @brief The exit message displayed to users when the shell terminates.
 *
 * This constant defines the message that is printed when the user exits the shell.
 * Example usage:
 */
#define EXIT_MESSAGE "See you later."


/** 
 * @def SHELL_NAME
 * @brief The shell prompt displayed to users.
 *
 * This constant defines the prompt symbol for the shell interface.
 * Example usage:
 * @code
 * printf("%s", SHELL_NAME);  // Displays the shell prompt
 * @endcode
 */
#define SHELL_NAME "$= "

/** 
 * @def EXIT_COMMAND
 * @brief The command to terminate the shell.
 *
 * This constant holds the string that users must enter to exit the shell program.
 * Example usage:
 * @code
 * if (strcmp(user_input, EXIT_COMMAND) == 0) {
 *     exit(0);
 * }
 * @endcode
 */
#define EXIT_COMMAND "exit"

/** 
 * @def WELCOME_MESSAGE
 * @brief A friendly greeting displayed when the shell starts.
 *
 * This constant holds a welcome message displayed at the beginning of the shell session.
 * Example usage:
 * @code
 * printf("%s", WELCOME_MESSAGE);  // Display the welcome message
 * @endcode
 */
#define WELCOME_MESSAGE "Welcome to bad_bash!\nBy Kevin & Axyl\nEnjoy your stay!"

/**
 * @brief Boolean definitions for compatibility with older C standards.
 *
 * These macros provide basic boolean logic using `int` since older C standards
 * do not natively support `bool`. 
 */
#define false 0  /**< Represents the boolean value false. */
#define true 1   /**< Represents the boolean value true. */
#define bool int /**< Defines `bool` as an alias for `int`. */

/**
 * @brief Constants for string sizes and command handling.
 */
#define MAX_STR_SIZE 256      /**< Maximum length for a single string. */
#define MAX_CMD_TOKENS 128    /**< Maximum number of tokens per command. */
#define MAX_ARGS 32           /**< Maximum number of arguments allowed per command. */

/**
 * @brief Exit status codes for process handling.
 *
 * These codes indicate whether a program exited successfully or encountered an error.
 */
#define EXIT_FAILURE -1  /**< Indicates that the program or task failed. */
#define EXIT_SUCCESS 0   /**< Indicates that the program or task completed successfully. */


/**
 * @brief Character constants used for string splitting and command parsing.
 *
 * These constants represent special symbols typically used in shell commands.
 */
#define SPACE ' '            /**< Represents a space character, used to separate tokens. */
#define INPUT_REDIR '<'      /**< Input redirection symbol. */
#define OUTPUT_REDIR '>'     /**< Output redirection symbol. */
#define PIPE '|'             /**< Pipe symbol, used to chain commands. */
#define RUN_IN_BKG '&'       /**< Background execution symbol. */

/**
 * @brief Constants for task management and memory usage.
 */
#define MAX_NUM_TASKS 10     /**< Initial maximum number of tasks (can be increased). */
#define MAX_TASKS 32         /**< Maximum number of tasks to allocate at runtime. */
#define MEMORY_POOL_SIZE 4096 /**< Size of the memory pool for task allocations (4 KB). */

/**
 * @brief Environment variable handling buffer size.
 */
#define ENV_BUFFER 4096      /**< Buffer size for reading environment variables. */

/**
 * @brief Limit for the `run_task_tree` function.
 */
#define MAX_LISTS 16         /**< Maximum number of lists allowed by `run_task_tree`. */

/**
 * @def NULL
 * @brief Represents a null pointer constant in C.
 *
 * The `NULL` macro is used to represent a null pointer in C, which indicates that a pointer 
 * does not point to any valid memory location. It ensures type safety by explicitly casting 
 * the value `0` to a `(void*)` type, making it applicable across all pointer types.
 *
 * @note In older C standards, `NULL` was sometimes defined as `0` without an explicit cast. 
 * However, using `(void*)0` ensures better portability and type compatibility.
 *
 * ## Example Usage
 * @code
 * int *ptr = NULL;  // Initialize a pointer to NULL
 * if (ptr == NULL) {
 *     printf("The pointer is NULL.\n");
 * }
 * @endcode
 *
 * ## Related Information
 * - **Comparison with `nullptr` in C++:** Starting from C++11, `nullptr` replaces `NULL` to 
 *   avoid ambiguity between integer 0 and a null pointer.
 * - **Common Use Case:** It is recommended to initialize pointers to `NULL` to avoid accessing 
 *   uninitialized memory, which can lead to undefined behavior.
 * - **Equivalent to:** The integer constant `0` when used in a pointer context. However, 
 *   using `NULL` improves readability and indicates the intent that the value is a pointer.
 *
 * @see stddef.h, stdlib.h, stdio.h - Standard libraries that define `NULL`.
 */
//#define NULL ((void*)0)


#endif // DEFS_H

