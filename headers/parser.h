#ifndef PARSER_H
#define PARSER_H

#include "task.h"   // Task struct and related definitions
#include <stdbool.h> // For bool type

#define MAX_STR_SIZE 4096  /**< Maximum size for strings. */

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
 * @return 0 on success, -1 if input string is NULL or an error occurs.
 */
int split_str(char *str_to_split, char *tokens[], int *count, char split_on, char *original);

/**
 * @brief Parses an input string into an array of Task structures.
 *
 * This function interprets the input string and converts it into multiple Task objects,
 * each representing a command and its associated properties such as background execution,
 * input/output redirection, and pipeline stages.
 *
 * @param input_string The input string containing the command line.
 * @param tasks Array of Task structures to store parsed tasks.
 * @param task_count Pointer to an integer that stores the number of tasks parsed.
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

