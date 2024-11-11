#pragma once
#include <stddef.h>
#include <stdint.h>

// Initialize the page and block allocator using Limine's memory map
void InitializeAllocator();

// Allocate a specific number of bytes (up to the page size)
void* KernelAllocate(size_t bytes);

// Reallocate a previously allocated block
void* KernelReallocate(void* ptr, size_t bytes);

// Free a previously allocated block or page
void KernelFree(void* ptr);

// Allocate a clean (zeroed-out) block of memory
void* KernelCleanAllocate(size_t bytes);

// Get the total used memory for tracking purposes
uint64_t GetTotalUsedMem();

// Override new/delete operators
inline void* operator new(size_t size)          { return KernelAllocate(size); }
inline void* operator new[](size_t size)        { return KernelAllocate(size); }
inline void operator delete(void* ptr)          { KernelFree(ptr); }
inline void operator delete[](void* ptr)        { KernelFree(ptr); }
inline void operator delete(void*, size_t)      { }
inline void operator delete[](void*, size_t)    { }

