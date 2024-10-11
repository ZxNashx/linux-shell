int split_str(char *str_to_split, char *tokens[], int *count, char split_on);
// Structure to hold details of a command
typedef struct Task {
    char *args[MAX_CMD_TOKENS];    // Command and arguments
    char *input_file;              // Input redirection file
    char *output_file;             // Output redirection file
    int is_background;             // Background task flag
    struct Task *next;             // Pointer to the next task in a pipeline
} Task;