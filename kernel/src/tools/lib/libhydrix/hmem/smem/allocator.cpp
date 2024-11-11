
#include <hmem/smem/allocator.h>
#include <limine.h>
#include <hmem/smem/smem.h>

extern struct limine_memmap_response* limine_memmap_us;
extern struct limine_hhdm_response* hhdm_memmap;

#define PAGE_SIZE 4096
#define MAX_SLAB_SIZE 256

struct Slab {
    void* base;
    uint16_t block_size;
    uint16_t total_blocks;
    uint16_t free_blocks;
    uint8_t* bitmap;
    struct Slab* next;
};

static struct Slab* slab_list = nullptr;
static uint64_t total_used_memory = 0;

void InitializeAllocator() {
    // Iterate over the memory map provided by Limine
    for (size_t i = 0; i < limine_memmap_us->entry_count; i++) {
        struct limine_memmap_entry* entry = limine_memmap_us->entries[i];

        if (entry->type == LIMINE_MEMMAP_USABLE) {
            uintptr_t start_addr = entry->base + hhdm_memmap->offset;
            uintptr_t end_addr = entry->base + entry->length + hhdm_memmap->offset;

            // Initialize a slab per usable memory region
            while (start_addr < end_addr) {
                struct Slab* new_slab = reinterpret_cast<Slab*>(start_addr);
                new_slab->base = reinterpret_cast<void*>(start_addr + sizeof(Slab));
                new_slab->block_size = MAX_SLAB_SIZE;
                new_slab->total_blocks = PAGE_SIZE / MAX_SLAB_SIZE;
                new_slab->free_blocks = new_slab->total_blocks;
                new_slab->bitmap = reinterpret_cast<uint8_t*>(new_slab->base) + PAGE_SIZE;

                // Initialize bitmap and add to slab list
                memset(new_slab->bitmap, 0, new_slab->total_blocks);
                new_slab->next = slab_list;
                slab_list = new_slab;

                start_addr += PAGE_SIZE;
            }
        }
    }
}

void* KernelAllocate(size_t bytes) {
    if (bytes > MAX_SLAB_SIZE) return nullptr;

    for (Slab* slab = slab_list; slab != nullptr; slab = slab->next) {
        if (slab->block_size >= bytes && slab->free_blocks > 0) {
            for (size_t i = 0; i < slab->total_blocks; i++) {
                if (!(slab->bitmap[i / 8] & (1 << (i % 8)))) {
                    // Mark as used
                    slab->bitmap[i / 8] |= (1 << (i % 8));
                    slab->free_blocks--;
                    total_used_memory += slab->block_size;
                    return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(slab->base) + i * slab->block_size);
                }
            }
        }
    }
    return nullptr;
}

void* KernelReallocate(void* ptr, size_t bytes) {
    if (!ptr || bytes > MAX_SLAB_SIZE) return nullptr;

    KernelFree(ptr);
    return KernelAllocate(bytes);
}

void KernelFree(void* ptr) {
    for (Slab* slab = slab_list; slab != nullptr; slab = slab->next) {
        uintptr_t offset = reinterpret_cast<uintptr_t>(ptr) - reinterpret_cast<uintptr_t>(slab->base);

        if (offset < PAGE_SIZE && offset % slab->block_size == 0) {
            size_t block_index = offset / slab->block_size;

            // Free block if it's allocated
            if (slab->bitmap[block_index / 8] & (1 << (block_index % 8))) {
                slab->bitmap[block_index / 8] &= ~(1 << (block_index % 8));
                slab->free_blocks++;
                total_used_memory -= slab->block_size;
            }
            return;
        }
    }
}

void* KernelCleanAllocate(size_t bytes) {
    void* mem = KernelAllocate(bytes);
    if (mem) {
        memset(mem, 0, bytes);
    }
    return mem;
}

uint64_t GetTotalUsedMem() {
    return total_used_memory;
}