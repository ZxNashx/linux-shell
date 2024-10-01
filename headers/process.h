

#include "defs.h"

int run_process(char * process_name, char * arguments[], char * newenvp[], bool wait_for_finish);

#define MAX_LISTS 10  // Maximum number of process lists in the process tree
#define MAX_PROCESSES 10 

// Structure definition for a single process
typedef struct Process {
    int pid;  // Process ID
    char name[32];  // Process name
    struct Process *prev;  // Pointer to the previous process in the doubly linked list
    struct Process *next;  // Pointer to the next process in the doubly linked list
} Process;

// Structure definition for a list of processes (doubly linked list)
typedef struct ProcessList {
    Process *head;  // Pointer to the head process in the list
} ProcessList;

// Structure definition for the process tree, which contains an array of process lists
typedef struct ProcessTree {
    ProcessList lists[MAX_LISTS];  // Array of process lists
} ProcessTree;
