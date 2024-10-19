#ifndef DEFS_H
#define DEFS_H
/**
 * @def EXIT_MESSAGE
 * @brief The exit message displayed to users when the shell terminates.
 *
 * This constant defines the message that is printed when the user exits the shell.
 */
#define EXIT_MESSAGE "See you later!"


/** 
 * @def SHELL_NAME
 * @brief The shell prompt displayed to users.
 *
 * This constant defines the prompt symbol for the shell interface.
 */
#define SHELL_NAME "$= "

/** 
 * @def EXIT_COMMAND
 * @brief The command to terminate the shell.
 *
 * This constant holds the string that users must enter to exit the shell program.
 */
#define EXIT_COMMAND "exit"

/** 
 * @def WELCOME_MESSAGE
 * @brief A friendly greeting displayed when the shell starts.
 *
 * This constant holds a welcome message displayed at the beginning of the shell session.
 */
#define WELCOME_MESSAGE "Welcome to bad_bash!\nBy Kevin & Axyl\nEnjoy your stay!"

/**
 * @brief Boolean definitions for compatibility with older C standards.
 *
 * These macros provide basic boolean logic using `int` since older C standards
 * do not natively support `bool`, and we want to minimize use of the C standard library.
 */
#define false 0  /**< Represents the boolean value false. */
#define true 1   /**< Represents the boolean value true. */
#define bool int /**< Defines `bool` as an alias for `int`. */

/**
 * @brief Constants for string sizes and command handling.
 */
#define MAX_STR_SIZE 512      /**< Maximum length for a single string. */
#define MAX_CMD_TOKENS 128    /**< Maximum number of tokens per command. */
#define MAX_ARGS 32           /**< Maximum number of arguments allowed per command. */
#define MAX_CMD_LENGTH 256    /**< Maximum length for a single command. */

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


#endif // DEFS_H

