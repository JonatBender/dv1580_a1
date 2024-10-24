
#include <stdlib.h>>
#include <stdbool.h>

size_t poolSize;
void* memPool;

typedef struct Block
{
    bool isAllocated;
    void* address;
    size_t size;
    struct Block* nextBlock;
} Block;

Block* listHead;

// Initialisation
void mem_init(size_t size)
// Initialise the memory manager with a memory pool of the given size
{
    poolSize = size;
    memPool = malloc(size);
    listHead->isAllocated = false;
    listHead->address = memPool;
    listHead->size = poolSize;
    listHead->nextBlock = NULL;
}

// Allocation
void* mem_alloc(size_t size)
// Allocates a block of memory of the specified size
// Find suitable vacant block in pool
// Mark as allocated
// Return pointer to start of the allocated block
{
    Block* walker = listHead;
    Block* prevWalker = NULL;
    void* poolWalker = memPool;
    while (walker != NULL)
    {
        if (walker->size >= size && !walker->isAllocated)
        {
            walker->isAllocated = true;
            walker->address = poolWalker;
            walker->nextBlock = walker + 1;
            if (prevWalker != NULL)
            {
                prevWalker->nextBlock = walker;
            }
            printf("Block allocated at %p", poolWalker);
            return poolWalker;
        }
        prevWalker = walker;
        walker = walker->nextBlock;
        if (walker != NULL)
        {
            poolWalker = prevWalker + prevWalker->size;
        }
        
    }
}

// Deallocation
void mem_free(void* block)
// Frees the specified block of memory
{
    if (block == NULL)
    {
        /* Print error message */
        printf("Invalid address");
    }
    else
    {
        Block* walker = listHead;
        Block* prevWalker = NULL;
        while (walker != NULL)
        {
            if (walker->address == block)
            {
                walker->isAllocated = false;
                // Join any adjacent empty blocks
                if (!prevWalker->isAllocated)
                {
                    prevWalker->size = prevWalker->size + walker->size;
                    prevWalker->nextBlock = walker->nextBlock;
                    walker = prevWalker;
                }
                if (!walker->nextBlock->isAllocated)
                {
                    walker->size = walker->size + walker->nextBlock->size;
                    walker->nextBlock = walker->nextBlock->nextBlock;
                }
                return;
            }
            prevWalker = walker;
            walker = walker->nextBlock;
        }
        printf("Block could not be found");
    }
}

// Resize
void* mem_resize(void* block, size_t size)
// Resizes the memory block to the specified size
// This might result in the memory block moving
{
    Block* walker = listHead;
    Block* prevWalker = NULL;
    while (walker != NULL)
    {
        if (walker->address == block)
        {
            // Found block
            if (size < walker->size)
            {
                // Free ending memory, creating new empty block
                size_t oldSize = walker->size;
                walker->size = size;
                Block* prevNext = walker->nextBlock;
                walker->nextBlock = walker + 1;
                walker->nextBlock->isAllocated = false;
                walker->nextBlock->address = (char*)walker->address + walker->size;
                if (!prevNext->isAllocated)
                {
                    walker->nextBlock->size = oldSize - size + prevNext->size;
                    walker->nextBlock->nextBlock = prevNext->nextBlock;
                }
                else
                {
                    walker->nextBlock->size = oldSize - size;
                    walker->nextBlock->nextBlock = prevNext;
                }                
            }
            else if (size > walker->size)
            {
                if (!walker->nextBlock->isAllocated && walker->size + walker->nextBlock->size >= size)
                {
                    size_t sumSize = walker->size + walker->nextBlock->size;
                    walker->size = size;
                    walker->nextBlock = walker + 1;
                    walker->nextBlock->size = sumSize - walker->size;
                    walker->nextBlock->address = (char*)walker->address + size;
                    walker->nextBlock->isAllocated = false;
                }
            }
        }
        prevWalker = walker;
        walker = walker->nextBlock;
    }
}

// Deinitialisation
void mem_deinit()
// Frees the memory pool allocated when initialising the memory manager
{
    free(memPool);
    poolSize = 0; // Redundant
}