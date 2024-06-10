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
#include "tools/lib/libhydrix/hmem/smem/smem.h"
#include "tools/lib/libhydrix/hmem/smem/heap.h"
#include "tools/lib/libhydrix/hlow/fpu/fpu.h"
#include "tools/lib/libhydrix/hkey/bkey.h"
#include "tools/lib/libhydrix/hdisplay/hdisplay.h"
#include "tools/lib/libhydrix/sdefs.h"
#include "tools/lib/libhydrix/hgl/fonts.h"
#include "tools/basic.h"

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
#define __KERNEL__BEFORE__START__TIME 4000
char* ThreeStrCat(string one, string two, string three)
{
    return strcat(one, strcat(two, three));
}

BMPI test_bmp;
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
    display.width = framebuffer->width;
    display.height = framebuffer->height;
    display.bpp = framebuffer->bpp;
    display.address = framebuffer->address;
    //one after the framebuffer
    heap_init((uint64_t)framebuffer->address + framebuffer->width * framebuffer->height + 2056);
    graphics.initgmgr((uint32_t*)framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch);
    
    console.graphics = graphics;
    console.currentline = 0;
    console.pxlinedown = 16;
    
    graphics.clear();
    BMPI tridentstartup;
    tridentstartup.data = (int*)tridentfull;
    tridentstartup.height = TRIDENTFULL_HEIGHT;
    tridentstartup.width = TRIDENTFULL_WIDTH;
    console.Clear();
    graphics.put_image((display.width / 2) - 250, (display.height / 2) - 250, tridentstartup);
    //if type is 0, add to memsize
    for (size_t i = 0; i < memmap->entry_count; i++)
    {
        if (memmap->entries[i]->type == 0)
        {
            memsize += memmap->entries[i]->length;
        }
    }  
    console.WriteLine("[GuardianOS Version: 0.0.1b]", rgb(170, 170, 255));
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
    //print memory size in MB
    console.WriteLine(ThreeStrCat("Memory Size: ", to_string(memsize / 1024 / 1024), " MB"), rgb(170, 255, 170));
    //print in bytes too
    console.WriteLine(ThreeStrCat("Memory Size: ", to_string(memsize / 1024), " KB"), rgb(170, 255, 170));
    console.WriteLine(ThreeStrCat("Memory Size: ", to_string(memsize), " B"), rgb(170, 255, 170));
    //capture
    graphics.Swap();
    RudamentaryWait(__KERNEL__BEFORE__START__TIME);
    graphics.clear();
    console.ClearS();
    console.WriteLine("This is screen one!", rgb(170, 255, 170));

    //Halt Forever [END OF KERNEL CODE]
    hcf();
    
}
