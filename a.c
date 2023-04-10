#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 1024 // total size of memory
#define PROCESS_SIZE 64 // size of each process
#define TIME_UNITS 10 // number of time units to run simulation

// memory allocation block
struct memory_block {
    int id; // process ID
    int size; // size of process
    int start_address; // starting address of process
};

// function to initialize memory blocks
void initialize_memory(struct memory_block memory[], int memory_size) {
    for (int i = 0; i < memory_size; i++) {
        memory[i].id = -1;
        memory[i].size = 0;
        memory[i].start_address = i;
    }
}

// function to allocate memory using the Best-Fit algorithm
void allocate_memory(struct memory_block memory[], int memory_size, int process_id, int process_size) {
    int best_fit_index = -1;
    int best_fit_size = MEMORY_SIZE;

    // find the best fit block for the process
    for (int i = 0; i < memory_size; i++) {
        if (memory[i].id == -1 && memory[i].size >= process_size && memory[i].size < best_fit_size) {
            best_fit_index = i;
            best_fit_size = memory[i].size;
        }
    }

    // allocate memory to the process
    if (best_fit_index != -1) {
        memory[best_fit_index].id = process_id;
        memory[best_fit_index].size = process_size;

        // split the block if necessary
        if (memory[best_fit_index].size < MEMORY_SIZE - 1) {
            memory[best_fit_index + memory[best_fit_index].size].id = -1;
            memory[best_fit_index + memory[best_fit_index].size].size = 0;
            memory[best_fit_index + memory[best_fit_index].size].start_address = best_fit_index + memory[best_fit_index].size;
        }
    }
}

// function to free memory used by a process
void free_memory(struct memory_block memory[], int memory_size, int process_id) {
    for (int i = 0; i < memory_size; i++) {
        if (memory[i].id == process_id) {
            memory[i].id = -1;
            memory[i].size = 0;

            // merge adjacent free blocks
            if (i > 0 && memory[i - 1].id == -1) {
                memory[i - memory[i - 1].size].size += memory[i].size;
                memory[i].size = 0;
            }
            if (i < memory_size - 1 && memory[i + 1].id == -1) {
                memory[i].size += memory[i + memory[i].size].size;
                memory[i + memory[i].size].size = 0;
            }
        }
    }
}

// function to calculate fragmentation and wasted blocks
void calculate_fragmentation(struct memory_block memory[], int memory_size, int *fragmentation, int *wasted_blocks) {
    int free_blocks = 0;
    int total_free_space = 0;

    for (int i = 0; i < memory_size; i++) {
        if (memory[i].id == -1) {
            free_blocks++;
            total_free_space += memory[i].size;
        }
    }

    *fragmentation = free_blocks - 1;
    *wasted_blocks = total_free_space / PROCESS_SIZE;
}

int main() {
    struct memory_block memory[MEMORY_SIZE];
    int time = 0; // current time unit

    // initialize memory blocks
    initialize_memory(memory, MEMORY_SIZE);

    // allocate initial processes
    allocate_memory(memory, MEMORY_SIZE, 0, PROCESS_SIZE);
    allocate_memory(memory, MEMORY_SIZE, 1, PROCESS_SIZE * 2);
    allocate_memory(memory, MEMORY_SIZE, 2, PROCESS_SIZE * 3);

    // run simulation for TIME_UNITS time units
    for (int t = 0; t < TIME_UNITS; t++) {
        // free memory used by processes 0 and 2
        free_memory(memory, MEMORY_SIZE, 0);
        free_memory(memory, MEMORY_SIZE, 2);

        // allocate new processes
        allocate_memory(memory, MEMORY_SIZE, t + 3, PROCESS_SIZE * (t % 4 + 1));

        // calculate fragmentation and wasted blocks
        int fragmentation, wasted_blocks;
        calculate_fragmentation(memory, MEMORY_SIZE, &fragmentation, &wasted_blocks);

        // print current state of memory
        printf("Time %d:\n", time);
        for (int i = 0; i < MEMORY_SIZE; i++) {
            if (memory[i].id == -1) {
                printf("Free block of size %d starting at address %d\n", memory[i].size, memory[i].start_address);
            } else {
                printf("Process %d of size %d starting at address %d\n", memory[i].id, memory[i].size, memory[i].start_address);
            }
        }
        printf("Fragmentation: %d\n", fragmentation);
        printf("Wasted blocks: %d\n", wasted_blocks);

        time++;
    }

    return 0;
}