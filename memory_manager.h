// Initialisation
void mem_init(size_t size);
// Initialise the memory manager with a memory pool of the given size

// Allocation
void* mem_alloc(size_t size);
// Allocates a block of memory of the specified size
// Find suitable vacant block in pool
// Mark as allocated
// Return pointer to start of the allocated block

// Deallocation
void mem_free(void* block);
// Frees the specified block of memory

// Resize
void* mem_resize(void* block, size_t size);
// Resizes the memory block to the specified size
// This might result in the memory block moving

// Deinitialisation
void mem_deinit();
// Frees the memory pool allocated when initialising the memory manager