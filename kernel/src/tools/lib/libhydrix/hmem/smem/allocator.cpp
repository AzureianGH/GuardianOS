#include <hmem/smem/allocator.h>
#include <limine.h>
#include <hmem/smem/smem.h> // For memset and memcpy

uint64_t* LocationOfLastAllocation;
uint64_t* LocationOfLastFree;

typedef struct mem_block {
    uint64_t size;
    struct mem_block* next;
} mem_block_t;

static void* mem_heap_base;
static void* mem_heap_end;
static mem_block_t* free_list = NULL;
uint64_t AddressOfMemStart;
uint64_t AddressOfMemEnd;

void InitializeHeap(unsigned long long memsize, unsigned long long EndOfMemSize) {
    mem_heap_base = (void*)memsize;
    mem_heap_end = mem_heap_base;
    AddressOfMemStart = (uint64_t)mem_heap_base;
    AddressOfMemEnd = (uint64_t)EndOfMemSize;
}

void* KernelAllocate(uint64_t bytes) {
    mem_block_t* prev = NULL;
    mem_block_t* curr = free_list;

    // Align the requested size to the size of mem_block_t
    bytes = (bytes + sizeof(mem_block_t) - 1) & ~(sizeof(mem_block_t) - 1);

    // Look for a free block that is large enough
    while (curr) {
        if (curr->size >= bytes) {
            // Remove from free list
            if (prev) {
                prev->next = curr->next;
            } else {
                free_list = curr->next;
            }

            // Track allocation for debugging
            LocationOfLastAllocation = (uint64_t*)(curr + 1);

            return (void*)(curr + 1);
        }
        prev = curr;
        curr = curr->next;
    }

    // No suitable free block, allocate at the end of the heap
    mem_block_t* block = (mem_block_t*)mem_heap_end;
    mem_heap_end = (void*)((uint8_t*)mem_heap_end + bytes + sizeof(mem_block_t));

    block->size = bytes;
    LocationOfLastAllocation = (uint64_t*)(block + 1);

    return (void*)(block + 1);
}

void KernelFree(void* ptr) {
    if (!ptr) {
        return;
    }
    
    mem_block_t* block = (mem_block_t*)ptr - 1;
    LocationOfLastFree = (uint64_t*)block;

    // Insert into the free list in address order to enable merging
    mem_block_t* prev = NULL;
    mem_block_t* curr = free_list;
    while (curr && curr < block) {
        prev = curr;
        curr = curr->next;
    }

    // Merge with the next block if adjacent
    if (curr && (uint8_t*)block + block->size + sizeof(mem_block_t) == (uint8_t*)curr) {
        block->size += curr->size + sizeof(mem_block_t);
        block->next = curr->next;
    } else {
        block->next = curr;
    }

    // Merge with the previous block if adjacent
    if (prev && (uint8_t*)prev + prev->size + sizeof(mem_block_t) == (uint8_t*)block) {
        prev->size += block->size + sizeof(mem_block_t);
        prev->next = block->next;
    } else {
        if (prev) {
            prev->next = block;
        } else {
            free_list = block;
        }
    }
}

// Get freelist count
int GetFreeListCount() {
    int count = 0;
    mem_block_t* curr = free_list;
    while (curr) {
        count++;
        curr = curr->next;
    }
    return count;
}

// Get used list count
int GetUsedListCount() {
    int count = 0;
    mem_block_t* curr = free_list;
    while (curr) {
        count++;
        curr = curr->next;
    }
    return count;
}

void* KernelReallocate(void* ptr, uint64_t bytes) {
    if (!ptr) {
        return KernelAllocate(bytes);
    }

    mem_block_t* old_block = (mem_block_t*)ptr - 1;
    if (old_block->size >= bytes) {
        return ptr; // The current block is already large enough
    }

    void* newptr = KernelAllocate(bytes);
    if (newptr) {
        memcpy(newptr, ptr, old_block->size);
        KernelFree(ptr);
    }
    return newptr;
}

void* KernelCleanAllocate(uint64_t bytes) {
    void* ptr = KernelAllocate(bytes);
    // Zero out the memory
    memset(ptr, 0, bytes);
    return ptr;
}

uint64_t GetTotalUsedMem() {
    uint64_t total = 0;
    mem_block_t* curr = (mem_block_t*)mem_heap_base;

    while ((void*)curr < mem_heap_end) {
        if (curr->size > 0) { // Block is in use
            total += curr->size;
        }
        curr = (mem_block_t*)((uint8_t*)curr + curr->size + sizeof(mem_block_t));
    }

    return total;
}

float GetUsedMemPercentage() {
    return (float)GetTotalUsedMem() / (float)(AddressOfMemEnd - AddressOfMemStart) * 100.0f;
}

/// @brief Find unused blocks and add them to the free list
void CleanHeap() {
    mem_block_t* prev = NULL;
    mem_block_t* curr = (mem_block_t*)mem_heap_base;

    while ((void*)curr < mem_heap_end) {
        if (curr->size > 0) { // Block is in use
            prev = curr;
        } else { // Block is free
            if (prev) {
                prev->next = curr;
            } else {
                free_list = curr;
            }
            curr->size = 0; // Mark as free
        }
        curr = (mem_block_t*)((uint8_t*)curr + curr->size + sizeof(mem_block_t));
    }
}

uint64_t MemoryAllocationDebugger::GetAddressOfLastAllocation() {
    return (uint64_t)LocationOfLastAllocation;
}

uint64_t MemoryAllocationDebugger::GetAddressOfLastFree() {
    return (uint64_t)LocationOfLastFree;
}
