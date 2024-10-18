#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>

#include "basicio.h"
#include "shell.h"
#include "defs.h"
#include "task.h"
#include "process.h"
#include "parser.h"
#include "str.h"

int main(int argc, char *argv[]) {
    char read_buffer[128];

    if(argc > 1){
        if(kstrcmp(argv[1], "1") == true){
            kstrcpy("/bin/ls -al", read_buffer);
        }
        if(kstrcmp(argv[1], "2") == true){
            kstrcpy("/bin/ls -al & /bin/ls -al & /bin/ls -al & /bin/ls -al &", read_buffer);
        }
        if(kstrcmp(argv[1], "3") == true){
            kstrcpy("ls -al", read_buffer);
        }
    }else{
        kstrcpy("/bin/ls -al | /usr/bin/wc -w", read_buffer);
    }

	Task tasks[MAX_TASKS];
	int current_task_count = 0;
    parse(read_buffer, tasks, &current_task_count);
    link_tasks(tasks, current_task_count);
    run_task_tree(tasks, current_task_count);
    return 0;
}