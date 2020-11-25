#include <stdint.h>
#include "mem.h"

// http://www.sunshine2k.de/articles/coding/cmemalloc/cmemory.html#ch33

void memory_copy(uint8_t *source, uint8_t *dest, uint32_t nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

// The following code is licensed under MIT licence. For further details, see http://www.sunshine2k.de/license.html.

#define DYNAMIC_MEM_TOTAL_SIZE       4*1024
#define dynamic_mem_node_t_SIZE        sizeof(dynamic_mem_node_t)

typedef struct dynamic_mem_node {
    uint32_t size;
    uint8_t used; // TODO bool?
    struct dynamic_mem_node *next;
    struct dynamic_mem_node *prev;
} dynamic_mem_node_t;

static uint8_t dynamic_mem_area[DYNAMIC_MEM_TOTAL_SIZE];
static dynamic_mem_node_t *dynamic_mem_start; // = (dynamic_mem_node_t *) dynamic_mem_area;

void init_dynamic_mem() {
    dynamic_mem_start = (dynamic_mem_node_t *) dynamic_mem_area;
    dynamic_mem_start->size = DYNAMIC_MEM_TOTAL_SIZE - dynamic_mem_node_t_SIZE;
    dynamic_mem_start->next = NULL_POINTER;
    dynamic_mem_start->prev = NULL_POINTER;
}

void *mem_alloc(size_t size) {
    dynamic_mem_node_t *current_mem_block;
    dynamic_mem_node_t *best_mem_block;
    uint32_t best_mem_block_size;

    /* init current block to start of heap */
    current_mem_block = dynamic_mem_start;
    /* init best heap block */
    best_mem_block = (dynamic_mem_node_t *) NULL_POINTER;
    best_mem_block_size = DYNAMIC_MEM_TOTAL_SIZE + 1; /* init with invalid size */

    while (current_mem_block) {
        /* check if current block matches and fittest best (so far) */
        if ((!current_mem_block->used) &&
            (current_mem_block->size >= (size + dynamic_mem_node_t_SIZE)) &&
            (current_mem_block->size <= best_mem_block_size)) {
            best_mem_block = current_mem_block;
            best_mem_block_size = current_mem_block->size;
        }

        current_mem_block = current_mem_block->next;
    }

    if (best_mem_block != NULL_POINTER) {
        dynamic_mem_node_t *mem_node_allocate;

        /* found a matching block, split it up and return the top of the memory area to the user */
        /* the best matching block is decreased by the needed memory area*/
        best_mem_block->size = best_mem_block->size - size - dynamic_mem_node_t_SIZE;
        /* new heap node is after the current heap + the size of its control data + allocated memory size */
        mem_node_allocate = (dynamic_mem_node_t *) (((uint8_t *) best_mem_block) + dynamic_mem_node_t_SIZE +
                                                   best_mem_block->size);
        mem_node_allocate->size = size;
        mem_node_allocate->used = 1;
        mem_node_allocate->next = best_mem_block->next;
        mem_node_allocate->prev = best_mem_block;
        if (best_mem_block->next != NULL_POINTER) {
            /* next block exists */
            best_mem_block->next->prev = mem_node_allocate;
        }
        best_mem_block->next = mem_node_allocate;
        /* return pointer to memory of new heap node after control data */
        return (void *) ((uint8_t *) mem_node_allocate + dynamic_mem_node_t_SIZE);
    }

    return NULL_POINTER;
}

void mem_free(void *p) {
    if (p == NULL_POINTER) {
        return;
    }

    /* get actual heap node */
    dynamic_mem_node_t *current_block = (dynamic_mem_node_t *) ((uint8_t *) p - dynamic_mem_node_t_SIZE);

    if (current_block == NULL_POINTER) {
        return;
    }

    current_block->used = 0;

    /* check if we can merge with next block */
    if (current_block->next != NULL_POINTER) {
        if (!current_block->next->used) {
            /* add size of next block and its control data to current block */
            current_block->size += current_block->next->size;
            current_block->size += dynamic_mem_node_t_SIZE;

            /* remove next block */
            /* link current block to next-next block */
            current_block->next = current_block->next->next;
            /* link next-next block to current block if next-next block exists */
            if (current_block->next != NULL_POINTER) /* current_block->next points to next-next block already! */
            {
                current_block->next->prev = current_block;
            }
        }
    }

    /* check if we can merge with previous block */
    if (current_block->prev != NULL_POINTER) {
        if (!current_block->prev->used) {
            /* add size of freed memory block and its control data to previous block */
            current_block->prev->size += current_block->size;
            current_block->prev->size += dynamic_mem_node_t_SIZE;

            /* remove freed block from list */
            /* link previous block to next block */
            current_block->prev->next = current_block->next;
            /* link next block to previous block if next block exists */
            if (current_block->next != NULL_POINTER) {
                current_block->next->prev = current_block->prev;
            }
        }
    }
}
