#ifndef BASICIO_H
#define BASICIO_H

#include <unistd.h>
#include <fcntl.h>

// Moves data from one file descriptor to another
// Returns 0 on success, -1 on failure
int move_fd(int fd_source, int fd_dest);

// Prints a null-terminated string to standard output
// Returns the number of characters written
int print_string(char *char_arr);

// Prints the contents of a file to the specified destination (file descriptor)
// Returns 0 on success, -1 on failure
int print_file(char *file_name, int print_destination);

// Reads user input into a buffer until a newline is encountered or max_size is reached
// Returns the number of characters read
int read_input(char *fill_buffer, int max_size);

#endif // BASICIO_H
