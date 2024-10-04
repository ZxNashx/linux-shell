#include <stdio.h>
#include "defs.h"
#include "str.h"

char temp[MAX_STR_SIZE]; // Ensure that this size is sufficient for the input strings

int split_str(char *str_to_split, char *tokens[], int *count, char split_on) {
    // Check if the input string is null
    if (str_to_split == NULL || tokens == NULL || count == NULL) {
        return -1; // Error code for invalid input
    }

    // Temporary copy of the original string to safely manipulate
    kstrcpy(str_to_split, temp);

    char *current_position = temp;
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

    return 0; // Optionally return a status code
}
