#pragma once
#include <stddef.h>
#include <stdint.h>
void InitializeHeap(unsigned long long memsize, unsigned long long AddressOfMemEnd);
void* KernelAllocate(uint64_t bytes);
void KernelFree(void* ptr);
void* KernelReallocate(void* ptr, uint64_t bytes);
void* KernelCleanAllocate(uint64_t bytes);
uint64_t GetTotalUsedMem();
int GetUsedListCount();
void CleanHeap();
float GetUsedMemPercentage();

class MemoryAllocationDebugger
{
    public:
        static uint64_t GetAddressOfLastAllocation();
        static uint64_t GetAddressOfLastFree();
};

inline void* operator new(size_t size)          { return KernelAllocate(size); }
inline void* operator new[](size_t size)        { return KernelAllocate(size); }
inline void operator delete(void* ptr)         { KernelFree(ptr); }
inline void operator delete[](void* ptr)       { KernelFree(ptr); }
inline void operator delete(void*, long unsigned int) { }
inline void operator delete[](void*, long unsigned int) { }
