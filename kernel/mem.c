#include <stdbool.h>
#include <stdint.h>
#include "mem.h"
#include "../drivers/display.h"
#include "util.h"

// http://www.sunshine2k.de/articles/coding/cmemalloc/cmemory.html#ch33

void memory_copy(uint8_t *source, uint8_t *dest, uint32_t nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

/*
 * The following code is based on code licensed under MIT licence
 * and thus also licensed under MIT license I guess?
 * For further details, see http://www.sunshine2k.de/license.html.
 */
#define DYNAMIC_MEM_TOTAL_SIZE 4*1024
#define DYNAMIC_MEM_NODE_SIZE sizeof(dynamic_mem_node_t)

typedef struct dynamic_mem_node {
    uint32_t size;
    bool used;
    struct dynamic_mem_node *next;
    struct dynamic_mem_node *prev;
} dynamic_mem_node_t;

static uint8_t dynamic_mem_area[DYNAMIC_MEM_TOTAL_SIZE];
static dynamic_mem_node_t *dynamic_mem_start;

void init_dynamic_mem() {
    dynamic_mem_start = (dynamic_mem_node_t *) dynamic_mem_area;
    dynamic_mem_start->size = DYNAMIC_MEM_TOTAL_SIZE - DYNAMIC_MEM_NODE_SIZE;
    dynamic_mem_start->next = NULL_POINTER;
    dynamic_mem_start->prev = NULL_POINTER;
}

void print_dynamic_node_size() {
    char node_size_string[256];
    int_to_string(DYNAMIC_MEM_NODE_SIZE, node_size_string);
    print_string("DYNAMIC_MEM_NODE_SIZE = ");
    print_string(node_size_string);
    print_nl();
}

void print_dynamic_mem_node(dynamic_mem_node_t *node) {
    char size_string[256];
    int_to_string(node->size, size_string);
    print_string("{size = ");
    print_string(size_string);
    char used_string[256];
    int_to_string(node->used, used_string);
    print_string("; used = ");
    print_string(used_string);
    print_string("}; ");
}

void print_dynamic_mem() {
    dynamic_mem_node_t *current = dynamic_mem_start;
    print_string("[");
    while (current != NULL_POINTER) {
        print_dynamic_mem_node(current);
        current = current->next;
    }
    print_string("]\n");
}

void *find_best_mem_block(dynamic_mem_node_t *dynamic_mem, size_t size) {
    // initialize the result pointer with NULL and an invalid block size
    dynamic_mem_node_t *best_mem_block = (dynamic_mem_node_t *) NULL_POINTER;
    uint32_t best_mem_block_size = DYNAMIC_MEM_TOTAL_SIZE + 1;

    // start looking for the best (smallest unused) block at the beginning
    dynamic_mem_node_t *current_mem_block = dynamic_mem;
    while (current_mem_block) {
        // check if block can be used and is smaller than current best
        if ((!current_mem_block->used) &&
            (current_mem_block->size >= (size + DYNAMIC_MEM_NODE_SIZE)) &&
            (current_mem_block->size <= best_mem_block_size)) {
            // update best block
            best_mem_block = current_mem_block;
            best_mem_block_size = current_mem_block->size;
        }

        // move to next block
        current_mem_block = current_mem_block->next;
    }
    return best_mem_block;
}

void *mem_alloc(size_t size) {
    dynamic_mem_node_t *best_mem_block =
            (dynamic_mem_node_t *) find_best_mem_block(dynamic_mem_start, size);

    // check if we actually found a matching (free, large enough) block
    if (best_mem_block != NULL_POINTER) {
        // subtract newly allocated memory (incl. size of the mem node) from selected block
        best_mem_block->size = best_mem_block->size - size - DYNAMIC_MEM_NODE_SIZE;

        // create new mem node after selected node, effectively splitting the memory region
        dynamic_mem_node_t *mem_node_allocate = (dynamic_mem_node_t *) (((uint8_t *) best_mem_block) +
                                                                        DYNAMIC_MEM_NODE_SIZE +
                                                                        best_mem_block->size);
        mem_node_allocate->size = size;
        mem_node_allocate->used = true;
        mem_node_allocate->next = best_mem_block->next;
        mem_node_allocate->prev = best_mem_block;

        // reconnect the doubly linked list
        if (best_mem_block->next != NULL_POINTER) {
            best_mem_block->next->prev = mem_node_allocate;
        }
        best_mem_block->next = mem_node_allocate;

        // return pointer to newly allocated memory (right after the new list node)
        return (void *) ((uint8_t *) mem_node_allocate + DYNAMIC_MEM_NODE_SIZE);
    }

    return NULL_POINTER;
}

void *merge_next_node_into_current(dynamic_mem_node_t *current_mem_node) {
    dynamic_mem_node_t *next_mem_node = current_mem_node->next;
    if (next_mem_node != NULL_POINTER && !next_mem_node->used) {
        // add size of next block to current block
        current_mem_node->size += current_mem_node->next->size;
        current_mem_node->size += DYNAMIC_MEM_NODE_SIZE;

        // remove next block from list
        current_mem_node->next = current_mem_node->next->next;
        if (current_mem_node->next != NULL_POINTER) {
            current_mem_node->next->prev = current_mem_node;
        }
    }
    return current_mem_node;
}

void *merge_current_node_into_previous(dynamic_mem_node_t *current_mem_node) {
    dynamic_mem_node_t *prev_mem_node = current_mem_node->prev;
    if (prev_mem_node != NULL_POINTER && !prev_mem_node->used) {
        // add size of previous block to current block
        prev_mem_node->size += current_mem_node->size;
        prev_mem_node->size += DYNAMIC_MEM_NODE_SIZE;

        // remove current node from list
        prev_mem_node->next = current_mem_node->next;
        if (current_mem_node->next != NULL_POINTER) {
            current_mem_node->next->prev = prev_mem_node;
        }
    }
}

void mem_free(void *p) {
    // move along, nothing to free here
    if (p == NULL_POINTER) {
        return;
    }

    // get mem node associated with pointer
    dynamic_mem_node_t *current_mem_node = (dynamic_mem_node_t *) ((uint8_t *) p - DYNAMIC_MEM_NODE_SIZE);

    // pointer we're trying to free was not dynamically allocated it seems
    if (current_mem_node == NULL_POINTER) {
        return;
    }

    // mark block as unused
    current_mem_node->used = false;

    // merge unused blocks
    current_mem_node = merge_next_node_into_current(current_mem_node);
    merge_current_node_into_previous(current_mem_node);
}