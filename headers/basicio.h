#ifndef BASICIO_H
#define BASICIO_H

#include "defs.h"

/**
 * @brief Moves data from one file descriptor to another.
 * 
 * This function reads data from the source file descriptor and writes it to the 
 * destination file descriptor until all data is transferred or an error occurs.
 * 
 * @param fd_source The source file descriptor to read from.
 * @param fd_dest The destination file descriptor to write to.
 * @return EXIT_SUCCESS on success, -1 on error.
 */
int move_fd(int fd_source, int fd_dest);

/**
 * @brief Prints a string to standard output.
 * 
 * This function prints the given character array to standard output. If the 
 * `newline` parameter is true, it appends a newline character to the output.
 * 
 * @param char_arr The character array to print.
 * @param newline If true, append a newline character at the end.
 * @return The number of characters written, including the newline if added.
 */
int print_string(char *char_arr, bool newline);

/**
 * @brief Prints a string to standard error output.
 * 
 * This function prints the given character array to standard error output. 
 * 
 * @param char_arr The character array to print.
 * @return The number of characters written
 */
int print_error(char * char_arr);

/**
 * @brief Prints the contents of a file to a specified destination.
 * 
 * Opens the specified file and prints its contents to the given file descriptor.
 * 
 * @param file_name The name of the file to print.
 * @param print_destination The file descriptor to print the contents to.
 * @return EXIT_SUCCESS on success, -1 on error.
 */
int print_file(char *file_name, int print_destination);

/**
 * @brief Reads input from standard input into a buffer.
 * 
 * This function reads characters from standard input until a newline character 
 * is encountered or the buffer size limit is reached. It ensures that the string 
 * is properly null-terminated.
 * 
 * @param fill_buffer The buffer to store the input.
 * @param max_size The maximum size of the buffer.
 * @return The number of characters read, or a negative value on error.
 */
int read_input(char *fill_buffer, int max_size);

#endif  // BASICIO_H
