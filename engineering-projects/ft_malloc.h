#define _GNU_SOURCE
// system
#include <stdio.h>
#include <unistd.h>
// user
#include "../common/errorFunctions.h"

#define ALIGN8(x) (((x) + 7) & ~7)
#define BLOCK_SIZE ALIGN8(sizeof(Block))

typedef struct Block
{
    size_t size;
    int free;
    struct Block* next;
    struct Block* prev;
}Block;

Block* head = NULL;

void *ft_malloc(size_t size);
void ft_free(void* ptr);
void print_block_info(void* ptr, const char* name);