#include <cstdint>
#include <cstddef>
#include <limine.h>
#include "tools/lib/libhydrix/hgl/graphics.h"
#include "tools/lib/libhydrix/hrand/rand.h"
#include "tools/lib/libhydrix/hmath/floatmath.h"
#include "tools/lib/libhydrix/hcon/console.h"
#include "tools/lib/libhydrix/hstring/string.h"
#include "tools/lib/libhydrix/hgl/color.h"
#include "tools/lib/libhydrix/hmath/intmath.h"
#include "tools/lib/libhydrix/hmath/higherbitmath.h"
#include "tools/lib/libhydrix/hlow/gdt/gdt.h"
#include "cpuid.h"
#include "tools/basic.h"
#include "tools/lib/libhydrix/hmem/smem/smem.h"
#include "tools/lib/libhydrix/hmem/smem/heap.h"
//#include "tools/lib/libhydrix/hlow/idt/idt.h"
#include "tools/lib/libhydrix/hlow/fpu/fpu.h"
#include "tools/lib/libhydrix/hkey/bkey.h"
#include "tools/lib/libhydrix/hdisplay/hdisplay.h"
//#include "tools/low/gdt/gdt.h"
// Set the base revision to 2, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.
#define __asm__ __asm
namespace {

__attribute__((used, section(".requests")))
volatile LIMINE_BASE_REVISION(2);

}
DisplayInfo display;
// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

namespace {

__attribute__((used, section(".requests")))
volatile limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr
};
//request memory map
__attribute__((used, section(".requests")))
volatile limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr
};
}

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .cpp file, as seen fit.

namespace {

__attribute__((used, section(".requests_start_marker")))
volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
volatile LIMINE_REQUESTS_END_MARKER;

}

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .cpp file



// Halt and catch fire function.
namespace {

void hcf() {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

}

// The following two stubs are required by the Itanium C++ ABI (the one we use,
// regardless of the "Itanium" nomenclature).
// Like the memory functions above, these stubs can be moved to a different .cpp file,
// but should never be removed.
extern "C" {
    int __cxa_atexit(void (*)(void *), void *, void *) { return 0; }
    void __cxa_pure_virtual() { hcf(); }
}

// Extern declarations for global constructors array.
extern void (*__init_array[])();
extern void (*__init_array_end[])();

// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.
Graphics graphics;
Console console;
void RudamentaryWait(uint64_t wait)
{
    wait = wait * 1024 * 1024;
    for (uint64_t i = 0; i < wait; i++)
    {
        //do math functions to waste time in assembly
        __asm__ __volatile__("nop");
    }
}
void printlog(const char *str, int level, int x, int y)
{
    //level 0 = info, 1 = warning, 2 = error, 3 = critical, 4 = debug, 5 = ok
    switch (level)
    {
        //concatenate str with "[INFO] "
        case 0:
            graphics.put_string("[INFO] ", x, y, rgb(255, 255, 255));
            break;
    }
}
/// @brief This is the worst possible way to reboot a system.
void TripleFault()
{
    __asm__ __volatile__("int $0");
}
#define __KERNEL__BEFORE__START__TIME 4000
extern "C" unsigned long long dump_rax();
extern "C" unsigned long long dump_rbx();
extern "C" unsigned long long dump_rcx();
extern "C" unsigned long long dump_rdx();
extern "C" unsigned long long dump_rsi();
extern "C" unsigned long long dump_rdi();
extern "C" unsigned long long dump_rbp();
extern "C" unsigned long long dump_rsp();
void fatal_error(const char *str, int num)
{
    graphics.clear();
    //top center
    graphics.fill_screen(rgb(20, 0, 0));
    // FUCK MY LIFE BECAUSE THE STUPID FUCKING STRCAT ISNT WORKING WITHOUT A MEMORY ALLOCATOR AND I HAVE TO DO THIS SHIT
    // NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE
    console.WriteLineS("FATAL EXCEPTION HAS OCCURED AND THE OS HAS BEEN SHUT DOWN!", rgb(255, 100, 100));
    graphics.put_string((string)"ERROR: ", 0, 33, rgb(255, 100, 100));
    graphics.put_string((string)str, 90, 33, rgb(255, 200, 200));
    graphics.put_string((string)"INT #: ", 0, 66, rgb(255, 100, 100));
    graphics.put_string(to_string(num), 90, 66, rgb(255, 200, 200));
    graphics.put_string((string)"RAX: ", 0, 99, rgb(255, 100, 100));
    graphics.put_string(to_string((uint64_t)dump_rax()), 60, 99, rgb(255, 200, 200));
    graphics.put_string((string)"RBX: ", 0, 132, rgb(255, 100, 100));
    graphics.put_string(to_string((uint64_t)dump_rbx()), 60, 132, rgb(255, 200, 200));
    graphics.put_string((string)"RCX: ", 0, 165, rgb(255, 100, 100));
    graphics.put_string(to_string((uint64_t)dump_rcx()), 60, 165, rgb(255, 200, 200));
    graphics.put_string((string)"RDX: ", 0, 198, rgb(255, 100, 100));
    graphics.put_string(to_string((uint64_t)dump_rdx()), 60, 198, rgb(255, 200, 200));
    graphics.put_string((string)"RSI: ", 0, 232, rgb(255, 100, 100));
    graphics.put_string(to_string((uint64_t)dump_rsi()), 60, 231, rgb(255, 200, 200));
    graphics.put_string((string)"RDI: ", 0, 264, rgb(255, 100, 100));
    graphics.put_string(to_string((uint64_t)dump_rdi()), 60, 264, rgb(255, 200, 200));
    graphics.put_string((string)"RBP: ", 0, 297, rgb(255, 100, 100));
    graphics.put_string(to_string((uint64_t)dump_rbp()), 60, 297, rgb(255, 200, 200));
    graphics.put_string((string)"RSP: ", 0, 330, rgb(255, 100, 100));
    graphics.put_string(to_string((uint64_t)dump_rsp()), 60, 330, rgb(255, 200, 200));
    graphics.put_string((string)"Press any key to reboot...", 0, 363, rgb(255, 100, 100));
    graphics.Swap();
    //wait for key press
    Wait_For_Key();
    //reboot the bad way
    TripleFault();
}
char* ThreeStrCat(string one, string two, string three)
{
    return strcat(one, strcat(two, three));
}

extern "C" void _start() {
    FPU::Enable();
    
    
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Call global constructors.
    for (std::size_t i = 0; &__init_array[i] != __init_array_end; i++) {
        __init_array[i]();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == nullptr
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    limine_memmap_response *memmap = memmap_request.response;
    //memory in bytes from memmap
    size_t memsize = 0;
    //graphics.init(framebuffer[0]); 
    graphics.framebuffer = *framebuffer;
    display.width = framebuffer->width;
    display.height = framebuffer->height;
    display.bpp = framebuffer->bpp;
    display.address = framebuffer->address;
    //one after the framebuffer
    graphics.SwapBuffer = (uint32_t*)framebuffer->address + framebuffer->width * framebuffer->height;
    
    console.graphics = graphics;
    console.currentline = 0;
    console.pxlinedown = 30;
    graphics.clear();
    //if type is 0, add to memsize
    for (size_t i = 0; i < memmap->entry_count; i++)
    {
        if (memmap->entries[i]->type == 0)
        {
            memsize += memmap->entries[i]->length;
        }
    }  
    console.Clear();
    console.WriteLine("[GuardianOS Version: 0.0.1b]", rgb(170, 170, 255));
    //init heap
    console.WriteLine("Initializing Heap...", rgb(170, 170, 170));
    heap_init((uint64_t)framebuffer->address + (uint64_t)framebuffer->width * (uint64_t)framebuffer->height);
    console.WriteLine("Heap Initialized!", rgb(170, 255, 170));
    console.WriteLine("Initializing FPU...", rgb(170, 170, 170));
    if (FPU::Is_Enabled())
    {
        console.WriteLine("FPU Enabled!", rgb(170, 255, 170));
    }
    else
    {
        console.WriteLine("FPU Not Detected!", rgb(255, 170, 170));
    }
    console.WriteLine("Initializing RNG...", rgb(170, 170, 170));
    rand_seed = (long int)framebuffer;
    console.WriteLine("RNG Initialized!", rgb(170, 255, 170));
    //gdt
    console.WriteLine("Initializing GDT...", rgb(170, 170, 170));
    
    gdt_init();
    console.WriteLine("GDT Initialized!", rgb(170, 255, 170));
    console.WriteLine("Initialization Complete!", rgb(170, 255, 170));
    graphics.Swap();
    RudamentaryWait(__KERNEL__BEFORE__START__TIME);
    graphics.clear();
    console.ClearS();
    console.WriteLine("Welcome to GuardianOS!", rgb(170, 255, 170));
    //print resolution, use strcat and threecat
    console.WriteLine(strcat(strcat("Resolution: ", to_string(display.width)), strcat("x", to_string(display.height))), rgb(170, 255, 170));
    //print all framebuffers
    console.WriteLine(strcat("Framebuffer Count: ", to_string(framebuffer_request.response->framebuffer_count)), rgb(170, 255, 170));
    //if framebuffer count is above 1, print Multiple Framebuffers Detected
    if (framebuffer_request.response->framebuffer_count > 1)
    {
        console.WriteLine("Multiple Framebuffers Detected!", rgb(170, 255, 170));
    }
    //print memory size in MB
    console.WriteLine(ThreeStrCat("Memory Size: ", to_string(memsize / 1024 / 1024), " MB"), rgb(170, 255, 170));
    //print in bytes too
    console.WriteLine(ThreeStrCat("Memory Size: ", to_string(memsize / 1024), " KB"), rgb(170, 255, 170));
    console.WriteLine(ThreeStrCat("Memory Size: ", to_string(memsize), " B"), rgb(170, 255, 170));
    graphics.Swap();
    hcf();
}