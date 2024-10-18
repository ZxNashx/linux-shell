#include <stdio.h>
#include "defs.h"
#include "str.h"
#include "task.h"

//char temp[MAX_STR_SIZE]; // Ensure that this size is sufficient for the input strings

int split_str(char *str_to_split, char *tokens[], int *count, char split_on, char *original) {
    
    bool delim_reached = false; //to make sure we capture all arguments for a specific cmd

    // Check if the input string is null
    if (str_to_split == NULL) {
        return -1; // Error code for invalid input
    }
    // Temporary copy of the original string to safely manipulate
    kstrcpy(str_to_split, original);

    char *current_position = original;
    int token_index = 0;

    // Ensure the count is initialized
    *count = 0;

    while (*current_position == SPACE) {
        current_position++; //to remove all leading white space before the first arg
    }
    tokens[token_index] = current_position;

    // Iterate through the string until the end is reached
    while (*current_position != '\0') {
        // Boundary check to avoid overflow
        if (token_index >= MAX_CMD_TOKENS) {
            printf("Error: Too many tokens. Maximum allowed is %d.\n", MAX_CMD_TOKENS);
            break; // Avoid overflow and stop if we exceed the number of allowed tokens
        }

        // Assign the current position as the start of a new token

        // Move to the end of the token or string
        while (*current_position != split_on && *current_position != '\0' && *current_position != SPACE) {
            current_position++;
        }

        // If we reach the end of the string, break the loop
        if (*current_position == '\0') {
            break;
        }

        //handles string = 'str &' -> only 'str' is saved not 'str '
        while (split_on != SPACE && *current_position == SPACE && *(current_position + 1) == split_on) {
            *current_position = '\0';
            delim_reached = true;
            current_position++;
        }

        // Replace the delimiter with a null terminator
        if (*current_position == split_on) {
            *current_position = '\0';
            delim_reached = true;
            current_position++; // Move past the delimiter
        }

        //skip all white space in between delimiters if split_on != ' '
        while ((split_on != SPACE) && *current_position == SPACE) {
            current_position++;
        }
        

        // Move to the next token position in the array
        if (delim_reached) {
            delim_reached = false;
            tokens[++token_index] = current_position;
        }
    }

    // Add the last token if necessary
    if (*current_position == '\0' && (current_position != tokens[token_index])) {
        if (token_index < MAX_CMD_TOKENS) {
            token_index++;
        }
    }

    *count = token_index;
    // Print out the tokens for debugging
    for (int i = 0; i < token_index; i++) {
        printf("Token[%d]: %s, length %i\n", i, tokens[i], kstrlen(tokens[i]));
    }
    return 0;
}

/*
int tokenize(char *str_to_split, char *tokens[], int *count, char split_on) {
    char temp_str[MAX_STR_SIZE];
    
    // Check if the input string is null
    if (str_to_split == NULL) {
        return -1; // Error code for invalid input
    }
    // Temporary copy of the original string to safely manipulate
    kstrcpy(str_to_split, temp_str);

    printf("%s", temp_str);
    
    int token_index = 0;

    // Ensure the count is initialized
    *count = 0;

    // Iterate through the string until the end is reached
    while (*current_position != '\0') {
        // Boundary check to avoid overflow
        if (token_index >= MAX_CMD_TOKENS) {
            printf("Error: Too many tokens. Maximum allowed is %d.\n", MAX_CMD_TOKENS);
            break; // Avoid overflow and stop if we exceed the number of allowed tokens
        }

        // Assign the current position as the start of a new token
        tokens[token_index] = current_position;

        // Move to the end of the token or string
        while (*current_position != split_on && *current_position != '\0') {
            current_position++;
        }

        // If we reach the end of the string, break the loop
        if (*current_position == '\0') {
            break;
        }

        // Replace the delimiter with a null terminator
        *current_position = '\0';
        current_position++; // Move past the delimiter

        // Move to the next token position in the array
        token_index++;
    }

    // Add the last token if necessary
    if (*current_position == '\0' && (current_position != tokens[token_index])) {
        if (token_index < MAX_CMD_TOKENS) {
            token_index++;
        }
    }

    *count = token_index;

    // Print out the tokens for debugging
    for (int i = 0; i < token_index; i++) {
        printf("Token[%d]: %s\n", i, tokens[i]);
    }

    return 0;
    
}*/
void parse(char *input_string, Task tasks[], int *task_count) {
    char *p = input_string;
    Task *current_task = NULL;

    *task_count = 0;

    while (*p != '\0') {
        // Skip leading whitespace
        while (*p == ' ' || *p == '\t' || *p == '\n') {
            *p = '\0'; // Replace whitespace with '\0' to tokenize
            p++;
        }

        if (*p == '\0')
            break;

        if (*p == '&') {
            // Background execution
            if (current_task != NULL) {
                current_task->is_background = true;
            }
            p++; // Move past '&'
            current_task = NULL; // Reset to start a new task if any
            continue;
        } else if (*p == '|') {
            // Pipe symbol encountered
            if (current_task != NULL) {
                current_task->is_pipe = true; // Mark that this task pipes to the next
            }
            p++; // Move past '|'
            current_task = NULL; // Reset to start the next task
            continue;
        } else if (*p == '>') {
            // Output redirection
            p++; // Move past '>'
            // Skip whitespace
            while (*p == ' ' || *p == '\t' || *p == '\n') {
                p++;
            }
            if (*p == '\0') {
                // Error: expected filename
                break;
            }
            char *filename = p;
            // Advance to end of filename
            while (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\0') {
                p++;
            }
            if (*p != '\0') {
                *p = '\0'; // Null-terminate filename
                p++;
            }
            if (current_task != NULL) {
                current_task->output_filename = filename;
            }
            continue;
        } else if (*p == '<') {
            // Input redirection
            p++; // Move past '<'
            // Skip whitespace
            while (*p == ' ' || *p == '\t' || *p == '\n') {
                p++;
            }
            if (*p == '\0') {
                // Error: expected filename
                break;
            }
            char *filename = p;
            // Advance to end of filename
            while (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\0') {
                p++;
            }
            if (*p != '\0') {
                *p = '\0'; // Null-terminate filename
                p++;
            }
            if (current_task != NULL) {
                current_task->input_filename = filename;
            }
            continue;
        } else {
            // Regular token (command or argument)
            if (current_task == NULL) {
                // Start a new task
                if (*task_count >= MAX_TASKS) {
                    // Error: too many tasks
                    break;
                }
                current_task = &tasks[*task_count];
                // Initialize the new task
                current_task->arg_count = 0;
                current_task->input_fd = -1;
                current_task->output_fd = -1;
                current_task->input_filename = NULL;
                current_task->output_filename = NULL;
                current_task->is_background = false;
                current_task->is_pipe = false;
                current_task->next = NULL;
                current_task->prev = NULL;
                current_task->prev_pipe_fd[0] = -1;
                current_task->prev_pipe_fd[1] = -1;
                (*task_count)++;
            }
            // Add the token to args[]
            if (current_task->arg_count < MAX_CMD_TOKENS - 1) {
                current_task->args[current_task->arg_count] = p;
                current_task->arg_count++;
                current_task->args[current_task->arg_count] = NULL; // Ensure null-termination
            } else {
                // Error: too many arguments
                break;
            }
            // Advance p to the end of the token
            while (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\0' && *p != '&' && *p != '|' && *p != '<' && *p != '>') {
                p++;
            }
            if (*p != '\0') {
                // Null-terminate the token
                char special_char = *p;
                *p = '\0';
                p++;
                // If the special character is one of our control characters, we need to process it in the next loop
                if (special_char == '&' || special_char == '|' || special_char == '<' || special_char == '>') {
                    p--; // Step back to process this character in the next iteration
                }
            }
        }
    }
}

void link_tasks(Task tasks[], int task_count) {
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].is_pipe && (i + 1) < task_count) {
            // Link the current task to the next task
            tasks[i].next = &tasks[i + 1];
            tasks[i + 1].prev = &tasks[i];
            tasks[i + 1].is_pipe = true; // Mark the next task as part of a pipeline
        }
    }
}
