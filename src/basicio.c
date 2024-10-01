#include <unistd.h>
#include <fcntl.h>

#include "defs.h"


int move_fd(int fd_source, int fd_dest) {
    int return_code = 0;
    int read_size = 32; 
    char buffer[read_size];
    ssize_t bytes_read;

    while ((bytes_read = read(fd_source, buffer, read_size)) > 0) {
        ssize_t bytes_written = 0;
        ssize_t total_written = 0;

        while (total_written < bytes_read) {
            bytes_written = write(fd_dest, buffer + total_written, bytes_read - total_written);
            if (bytes_written < 0) {
                return_code = -1;
                break;
            }
            total_written += bytes_written;
        }

        if (bytes_written < 0) {
            break; 
        }
    }

    if (bytes_read < 0) {
        return_code = -1;
    }

    return return_code;
}

int print_string(char * char_arr, bool newline){
	int i = 0;
	int buff_size = 512;
	char buffer[buff_size];
	int done = false;
	do{
		if(char_arr[i] != '\0' && i < buff_size){
			buffer[i] = char_arr[i];
			i++;
		}else{
			done = true;
		}
	}while(done == false);

	if(newline){
		buffer[i] = '\n';
		write(STDOUT_FILENO, buffer, ++i);
	}else{
		write(STDOUT_FILENO, buffer, i);
	}
	return i;
}

int print_file(char *file_name, int print_destination) {
    int return_code = 0;
    int open_file = open(file_name, O_RDONLY);

    if (open_file != -1) {
        return_code = move_fd(open_file, print_destination);
        close(open_file); 
    } else {
        return_code = -1;
    }
    return return_code;
}

int read_input(char *fill_buffer, int max_size) {	
    char current_char = '\0';
    int i = 0;
    int read_return;

    // Read until newline or max_size limit is reached
    while (current_char != '\n' && i < max_size - 1) {
        read_return = read(0, &current_char, 1);
        if (read_return <= 0) break;  // Exit loop on error or EOF
        fill_buffer[i++] = current_char;
    }

    // Properly terminate the string
    fill_buffer[i] = '\0';

    // If we reached the limit, or there's still input left, clear the buffer
    while (current_char != '\n' && read_return > 0) {
        read_return = read(0, &current_char, 1);
    }

    // Return the number of characters read (negative if error occurred)
    return read_return < 0 ? -i : i;
}


