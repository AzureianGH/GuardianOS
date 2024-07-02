#include <stdint.h>
#include <stddef.h>
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
#include "tools/lib/libhydrix/hkey/keyboard.h"
#include "tools/lib/libhydrix/hdisplay/hdisplay.h"
#include "tools/lib/libhydrix/sdefs.h"
#include "tools/lib/libhydrix/hgl/fonts.h"
#include "tools/basic.h"
#include "tools/lib/libhydrix/hlow/idt/idt.h"
#include "tools/lib/libhydrix/hlow/idt/isr.h"
#include "tools/lib/libhydrix/hmouse/mouse.h"
#include "tools/lib/libhydrix/hsyscall/syscall.h"
#include "tools/lib/libhydrix/hlow/scheduler/scheduler.h"
#include "tools/lib/libhydrix/hlow/paging/paging.h"
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

__attribute__((used, section(".requests")))
volatile limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
    .response = nullptr
};


// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .cpp file, as seen fit.

namespace {

__attribute__((used, section(".requests_start_marker")))
volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
volatile LIMINE_REQUESTS_END_MARKER;

}



// Halt and catch fire function.
namespace {

void halt() {
    for (;;)
        asm ("hlt");
}

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
typedef struct {
        uint8_t byte;
        uint16_t word;
        uint32_t dword;
    } MyStruct;
// Extern declarations for global constructors array.
extern void (*__init_array[])();
extern void (*__init_array_end[])();

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
void testproc()
{
    while (true)
    {
        console.WriteLine("Test Process", rgb(170, 255, 170));
        RudamentaryWait(1000);
    }
}
BMPI test_bmp;
extern "C" void _start() {
    
    FPU::Enable();
    
    
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Call global constructors.
    for (size_t i = 0; &__init_array[i] != __init_array_end; i++) {
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
    limine_hhdm_response *hhdm = hhdm_request.response;
    Set_LIMINE_MEMMAP_PAGING(memmap->entries);
    uint64_t offsetofhhdm = hhdm->offset;
    //memory in bytes from memmap
    size_t memsize = 0;
    for (size_t i = 0; i < memmap->entry_count; i++)
    {
        if (memmap->entries[i]->type == 0)
        {
            memsize += memmap->entries[i]->length;
            
        }
    } 
    display.width = framebuffer->width;
    display.height = framebuffer->height;
    display.bpp = framebuffer->bpp;
    display.address = framebuffer->address;
    //one after the framebuffer
    //heap_init the offset of hhdm
    heap_init(offsetofhhdm);
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
    
    set_isr_console(&console);
    Keyboard_Init(&console);
    set_mouse_console(&console);
    syscall_init(&console);
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
    //idt
    
    
    console.WriteLine("Initializing IDT...", rgb(170, 170, 170));
    isr_install();
    enable_interrupts();
    set_pit_freq(1000); // 1000 Hz (1ms)
    console.WriteLine("IDT Initialized!", rgb(170, 255, 170));
    //paging
    console.WriteLine("Initializing Paging...", rgb(170, 170, 170));
    InitPML4();
    console.WriteLine("Paging Initialized!", rgb(170, 255, 170));

    
    //scheduler
    console.WriteLine("Initializing Scheduler...", rgb(170, 170, 170));

    // TODO: Implement the scheduler
    init_scheduler();

    console.WriteLine("Scheduler Initialized!", rgb(170, 255, 170));
    console.WriteLine("Initialization Complete!", rgb(170, 255, 170));
    console.WriteLine(ThreeStrCat("[GuardianOS Version: ", OS_Version_, "]"), rgb(170, 170, 255));
    graphics.Swap();
    RudamentaryWait(__KERNEL__BEFORE__START__TIME);
    graphics.clear();
    console.ClearS();
    console.WriteLine("Welcome to GuardianOS!", rgb(170, 255, 170));
    //print resolution, use strcat and threecat
    console.WriteLine(ThreeStrCat(ThreeStrCat("Resolution: ", to_string(display.width), strcat("x", to_string(display.height))), "x", to_string(framebuffer->bpp)), rgb(170, 255, 170));
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
    console.allow_typing = false;
    BMPA Cursor;
    Cursor.data = (long*)cnormal;
    Cursor.height = CNORMAL_HEIGHT;
    Cursor.width = CNORMAL_WIDTH;
    BMPI BGImg;
    BGImg.data = (int*)OSBG;
    BGImg.height = OSBG_HEIGHT;
    BGImg.width = OSBG_WIDTH;
    char* MouseStateStrings[] = {
        "MOUSE_NONE",
        "MOUSE_LEFT",
        "MOUSE_RIGHT",
        "MOUSE_MIDDLE",
        "MOUSE_SCROLL_UP",
        "MOUSE_SCROLL_DOWN"
    };
    while (true)
    {
        
        graphics.put_image_stretch(0, 0, display.width, display.height, BGImg);
        console.WriteLine(to_string(Get_Current_Mouse_State()), 0xFF0000);
        graphics.put_image_alpha(get_mouse_x(), get_mouse_y(), Cursor);
        graphics.Swap();
        console.Clear();
    }
    halt();
    
}
