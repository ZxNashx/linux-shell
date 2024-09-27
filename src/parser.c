#include <unistd.h>
#include <fcntl.h>
#include "defs.h"




/*

str_to_split: the string that will be split into char arrays based on the value of split_on
tokens: the array of strs resulting from str_to_split
count: the count of tokens
split_on: the character that the array will be split by

*/

char temp[MAX_STR_SIZE];
int split_str(char *str_to_split, char *tokens[], int *count, char split_on) {
    strcpy(str_to_split,temp);
    char *current_position = temp;
    int token_index = 0;
    
    // Ensure the count is initialized
    *count = 0;
    
    while (*current_position != '\0') {
        // Start of a new token
        tokens[token_index] = current_position;
        
        // Move till end of the token or string
        while (*current_position != split_on && *current_position != '\0') {
            current_position++;
        }

        // If end of string, break the loop
        if (*current_position == '\0') {
            break;
        }

        // Replace the delimiter with a null terminator
        *current_position = '\0';
        current_position++; // Move past the delimiter
        
        // Move to the next token position in the array
        token_index++;
    }

    // Increment count for the last token if not empty
    if (*current_position == '\0' && current_position != tokens[token_index]) {
        token_index++;
    }

    *count = token_index;
    return 0; // Optionally return a status code
}
