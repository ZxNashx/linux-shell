#include "defs.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "task.h"

static uint8_t memory_pool[MEMORY_POOL_SIZE];
static size_t memory_offset = 0;  

static Task *task_pool[MAX_TASKS];
static int task_pool_index = 0;  


void task_memory_init() {
    memory_offset = 0;  
    task_pool_index = 0;
    for (int i = 0; i < MAX_TASKS; i++) {
        task_pool[i] = NULL;  
    }
}


void *alloc_memory(size_t size) {
    if (memory_offset + size > MEMORY_POOL_SIZE) {
        return NULL; 
    }
    void *allocated_memory = &memory_pool[memory_offset];
    memory_offset += size;
    return allocated_memory;
}

Task *task_alloc() {
    if (task_pool_index >= MAX_TASKS) {
        printf("Error: Maximum task allocation reached.\n");
        return NULL;
    }

    Task *new_task = (Task *)alloc_memory(sizeof(Task));
    if (!new_task) {
        printf("Error: Not enough memory in the pool for a new Task.\n");
        return NULL;
    }

    for (int i = 0; i < MAX_CMD_TOKENS; i++) {
        new_task->args[i] = NULL;
    }
    new_task->input_file = NULL;
    new_task->output_file = NULL;
    new_task->is_background = 0;
    new_task->next = NULL;

    task_pool[task_pool_index++] = new_task;
    return new_task;
}

void task_free(Task *task) {
    if (task == NULL) {
        return;
    }
    for (int i = 0; i < task_pool_index; i++) {
        if (task_pool[i] == task) {
            for (int j = i; j < task_pool_index - 1; j++) {
                task_pool[j] = task_pool[j + 1];
            }
            task_pool[--task_pool_index] = NULL;
            break;
        }
    }
}

void display_task_memory() {
    printf("Memory Pool Usage: %zu / %d bytes used.\n", memory_offset, MEMORY_POOL_SIZE);
    printf("Allocated Task structures: %d\n", task_pool_index);
    for (int i = 0; i < task_pool_index; i++) {
        printf("  Task %d at %p\n", i, (void *)task_pool[i]);
    }
}
