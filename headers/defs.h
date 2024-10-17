#define false 0
#define true 1
#define bool int

#define MAX_STR_SIZE 256
#define MAX_CMD_TOKENS 128
#define MAX_ARGS 10

#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0



//defines for splitting strings
#define SPACE ' '
#define INPUT_REDIR '<'
#define OUTPUT_REDIR '>'
#define PIPE '|'
#define RUN_IN_BKG '&'

// for tasks
#define MAX_NUM_TASKS 10

#define MEMORY_POOL_SIZE 4096 // Memory pool size for all task allocations (4 KB)
#define MAX_TASKS 32          // Maximum number of Task structures to allocate

#define ENV_BUFFER 4096


#define MAX_LISTS 16  // For run_task_tree's limit

