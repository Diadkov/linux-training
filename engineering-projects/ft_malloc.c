#define _GNU_SOURCE
#include "ft_malloc.h"

void *ft_malloc(size_t size)
{
    if(size == 0)
        return NULL;
    size = ALIGN8(size);

    if(head == NULL)
    {
        head = sbrk(BLOCK_SIZE + size);
        if(head == (void*)-1)
            return NULL;

        head->size = size;  
        head->free = 0;
        head->next = NULL;
        head->prev = NULL;
        
        return (void*)(head + 1);
    }
    // first fit
    Block* current = head;
    Block* prev = NULL;
    while(current)
    {
        if(current->free && current->size >= size)
        {
            if(current->size >= size + ALIGN8(1) + BLOCK_SIZE) // don't understand condition
            {
                char* raw = (char*)(current + 1);
                Block* newBlock = (Block*)(raw + ALIGN8(size));; // why char? 
                newBlock->size = current->size - size - BLOCK_SIZE;
                newBlock->free = 1;
                
                newBlock->next = current->next;
                newBlock->prev = current;

                if(current->next)
                    current->next->prev = newBlock; // what
                
                current->next = newBlock;
                current->size = size;
            }
            current->free = 0;
            return (void*)(current + 1);// next block 
        }
        prev = current;
        current = current->next;
    }
    
    Block* newBlock =  sbrk(BLOCK_SIZE + size);
    if(newBlock == (void*)-1)
        return NULL;

    newBlock->size = size;  
    newBlock->free = 0;
    newBlock->next = NULL;
    // link new block
    prev->next = newBlock;
    newBlock->prev = prev;

    return (void*)(newBlock + 1); // get data - not metadata 
}

void ft_free(void* ptr)
{
    if(!ptr)
        return;

    Block* metadata = (Block*)ptr - 1;
    if(metadata->free) // save from double free 
    {
        printf("Error. Double free detected");
        return;
    }
        
    metadata->free = 1;

    if(metadata->next && metadata->next->free) // forward merge
    {
        Block* next = metadata->next;
        
        metadata->size += BLOCK_SIZE + next->size;
        metadata->next = next->next;

        if (metadata->next)
            metadata->next->prev = metadata;
    }
    if(metadata->prev && metadata->prev->free) // backward merge
    {
        Block* prev = metadata->prev;

        prev->size += BLOCK_SIZE + metadata->size;
        prev->next = metadata->next;

        if (metadata->next)
            metadata->next->prev = prev;
    }
}

void print_block_info(void* ptr, const char* name)
{
    if (!ptr)
    {
        printf("%s = NULL\n", name);
        return;
    }

    Block* b = (Block*)ptr - 1;
    printf("%s -> addr: %p | size: %zu | free: %d\n",
           name, ptr, b->size, b->free);
}

int main()
{
    printf("===== TEST START =====\n\n");

    // 🔹 1. Basic alloc
    void* p1 = ft_malloc(100);
    void* p2 = ft_malloc(100);
    void* p3 = ft_malloc(100);

    print_block_info(p1, "p1");
    print_block_info(p2, "p2");
    print_block_info(p3, "p3");

    printf("\n--- Free middle (p2) ---\n");
    ft_free(p2);

    print_block_info(p2, "p2");

    printf("\n--- Allocate smaller (should reuse p2 + split) ---\n");
    void* p4 = ft_malloc(50);

    print_block_info(p4, "p4");

    printf("\n--- Free all ---\n");
    ft_free(p1);
    ft_free(p3);
    ft_free(p4);

    printf("All freed.\n");

    printf("\n--- Allocate big block (should reuse coalesced memory) ---\n");
    void* p5 = ft_malloc(250);

    print_block_info(p5, "p5");

    printf("\n===== TEST END =====\n");

    return 0;
}