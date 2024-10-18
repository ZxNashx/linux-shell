#ifndef PARSER_H
#define PARSER_H

#include "task.h"

/**
 * @brief Splits a string into tokens based on a delimiter.
 *
 * This function splits a string into multiple tokens based on the specified delimiter.
 * It also removes leading whitespace from tokens and ensures no overflow of tokens occurs.
 *
 * @param str_to_split The input string to split.
 * @param tokens Array of strings where tokens will be stored.
 * @param count Pointer to an integer that stores the number of tokens found.
 * @param split_on The delimiter character used for splitting the string.
 * @param original Temporary copy of the original string to safely manipulate.
 * @return EXIT_SUCCESS on success, -1 if input string is NULL or an error occurs.
 */
int split_str(char *str_to_split, char *tokens[], int *count, char split_on, char *original);

/**
 * @brief Parses the input string into tasks, handling commands, arguments, 
 *        and I/O redirections.
 *
 * This function takes an input string containing a shell command and breaks it
 * into multiple tasks based on the presence of special characters (such as `|`, `&`, 
 * `<`, `>`). It also initializes task-related metadata such as input and output 
 * filenames, background execution flags, and pipe settings.
 *
 * @param input_string The input string containing the command line to be parsed.
 *                     This string is modified in-place during parsing by replacing
 *                     whitespace and special characters with null terminators (`\0`).
 * @param tasks An array of Task structures where each parsed task will be stored.
 * @param task_count A pointer to an integer where the total number of parsed tasks
 *                   will be stored.
 *
 * @details
 * The function processes the input string character by character. It identifies:
 * - Background execution (`&`)
 * - Piping between tasks (`|`)
 * - Input redirection (`<`)
 * - Output redirection (`>`)
 * - Command and argument tokens
 *
 * It creates new tasks as needed, initializes their properties, and stores them 
 * in the provided `tasks` array. Arguments are stored in the `args` array within 
 * each Task structure, with proper null termination.
 *
 * @note
 * - The input string is modified in place. Each token and special character is 
 *   null-terminated.
 * - If the input contains too many tasks or arguments, an error message is printed, 
 *   and the parsing stops.
 *
 * Example usage:
 * @code
 * Task tasks[MAX_TASKS];
 * int task_count;
 * char input[] = "ls -al | grep txt > output.txt &";
 * parse(input, tasks, &task_count);
 * @endcode
 *
 * @error
 * - If there are too many tasks or arguments, an error is printed using `print_error()`.
 * - If a redirection operator is not followed by a filename, an error is printed.
 *
 * @see Task
 */
void parse(char *input_string, Task tasks[], int *task_count);

/**
 * @brief Links tasks in a pipeline by setting appropriate pointers.
 *
 * This function links tasks that are part of a pipeline by assigning pointers to the next
 * and previous tasks, and marks them as part of a pipeline.
 *
 * @param tasks Array of Task structures representing the parsed tasks.
 * @param task_count The number of tasks parsed.
 */
void link_tasks(Task tasks[], int task_count);

#endif // PARSER_H

