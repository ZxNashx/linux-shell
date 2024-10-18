#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include "defs.h"
#include "basicio.h"
#include "signal_handler.h"

void handle_sigint(int sig) {
    print_error("Please type 'exit' to close the shell.");
    print_string(SHELL_NAME, false);
}
