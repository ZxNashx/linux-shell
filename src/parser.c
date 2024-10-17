#include <stdio.h>
#include "defs.h"
#include "str.h"

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
