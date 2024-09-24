#include <unistd.h>
#include <fcntl.h>

#include "basicio.h"
#include "shell.h"
#include "defs.h"


int main(int argc, char * argv[]){
	int input_buffer_size = 256;
	char test[input_buffer_size];
	int result;
	result = read_input(test, input_buffer_size);

	print_string(test);
	return 0;
}