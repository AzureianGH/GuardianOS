#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <libhydrix/libhydrix.h>
#include <basic.h>

#define __asm__ __asm
namespace {

__attribute__((used, section(".requests")))
volatile LIMINE_BASE_REVISION(2);

}
DisplayInfo display;

namespace {

__attribute__((used, section(".requests")))
volatile limine_boot_time_request boot_time_request = {
    .id = LIMINE_BOOT_TIME_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".requests")))
volatile limine_bootloader_info_request bootloader_info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".requests")))
volatile limine_dtb_request dtb_request = {
    .id = LIMINE_DTB_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".requests")))
volatile limine_efi_memmap_request efi_memmap_request = {
    .id = LIMINE_EFI_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".requests")))
volatile limine_efi_system_table_request efi_system_table_request = {
    .id = LIMINE_EFI_SYSTEM_TABLE_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".requests")))
volatile limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".requests")))
volatile limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".requests")))
volatile limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".requests")))
volatile limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".requests")))
volatile limine_smbios_request smbios_request = {
    .id = LIMINE_SMBIOS_REQUEST,
    .revision = 0,
    .response = nullptr
};

}

namespace {

__attribute__((used, section(".requests_start_marker")))
volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
volatile LIMINE_REQUESTS_END_MARKER;

}
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
extern "C" {
    int __cxa_atexit(void (*)(void *), void *, void *) { return 0; }
    void __cxa_pure_virtual() { hcf(); }
}

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
BMPI rotate_bmpi(int deg, BMPI BMP_)
{
    BMPI BMP;
    BMP.data = BMP_.data;
    BMP.height = BMP_.height;
    BMP.width = BMP_.width;
    int* data = BMP.data;
    int* newdata = (int*)kcalloc(BMP.width * BMP.height * display.bpp / 8);
    int newx = 0;
    int newy = 0;
    for (int y = 0; y < BMP.height; y++)
    {
        for (int x = 0; x < BMP.width; x++)
        {
            newx = x;
            newy = y;
            if (deg == 90)
            {
                newx = y;
                newy = BMP.height - x;
            }
            else if (deg == 180)
            {
                newx = BMP.width - x;
                newy = BMP.height - y;
            }
            else if (deg == 270)
            {
                newx = BMP.width - y;
                newy = x;
            }
            newdata[newy * BMP.width + newx] = data[y * BMP.width + x];
        }
    }
    BMP.data = newdata;
    return BMP;
}

//reboot the bad way
void TripleFault()
{
    //idt reg set to null
    idt_register_t idt_reg;
    idt_reg.base = 0;
    idt_reg.limit = 0;
    //lidt

    asm volatile("lidtq %0" : "=m"(idt_reg));
}
typedef void (*SimpleProgramFunc)();
void read_bytes(const uint8_t* src, void* dest, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        ((uint8_t*)dest)[i] = src[i];
    }
}

void play(uint64_t freq)
{
    uint64_t divisor = 1193180 / freq;

    outb(0x43, 0xB6);

    uint8_t l = static_cast<uint8_t>(divisor);
    uint8_t h = static_cast<uint8_t>(divisor >> 8);

    outb(0x42, l);
    outb(0x42, h);

    outb(0x61, inb(0x61) | 0x3);
}

void stop()
{
    outb(0x61, inb(0x61) & 0xFC);
}

void beep(uint64_t freq, uint64_t msec)
{

    play(freq);
    msleep(msec);
    stop();
}

extern "C" void _start() {
    
    
    
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
    FPU::Enable();
    // Fetch the first framebuffer.
    limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    limine_memmap_response *memmap = memmap_request.response;
    limine_hhdm_response *hhdm = hhdm_request.response;
    uint64_t offsetofhhdm = hhdm->offset;
    //memory in bytes from memmap
    size_t memsize = _retrieve_total_memory(memmap);
    display.width = framebuffer->width;
    display.height = framebuffer->height;
    display.bpp = framebuffer->bpp;
    display.address = framebuffer->address;
    //one after the framebuffer
    //heap_init the offset of hhdm
    heap_init(offsetofhhdm);
    graphics.Init((uint32_t*)framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch);
    
    
    console.Init(&graphics, 16, false);
    
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
    Set_Console_PCI(&console);
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
    set_pit_freq(1000);
    time_init();
    console.WriteLine("IDT Initialized!", rgb(170, 255, 170));
    //paging
    console.WriteLine("Initializing Paging...", rgb(170, 170, 170));
    console.WriteLine("Paging Initialized!", rgb(170, 255, 170));

    
    //scheduler
    console.WriteLine("Initializing Scheduler...", rgb(170, 170, 170));

    console.WriteLine("Scheduler Initialized!", rgb(170, 255, 170));
    console.WriteLine("Initialization Complete!", rgb(170, 255, 170));
    console.WriteLine(ThreeStrCat("[GuardianOS Version: ", OS_Version_, "]"), rgb(170, 170, 255));
    graphics.swap();
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
    pci_init();
    //capture
    graphics.swap();
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
    int fps = 0;
    int frames = 0;
    int last = 0;
    Time_t time;
    Time_t Cent;
    time = getTime12Hour(EasternTime);
    Cent = getTime12Hour(CenteralEurope);
    BMPI Stretched = *stretch_image(&BGImg, display.width, display.height);
    while (true)
    {
        
        graphics.put_image(0, 0, Stretched);
        switch (Get_Current_Mouse_State())
        {
            case MouseState::MOUSE_NONE:
                console.WriteLine("None", 0xFF0000);
                break;
            case MouseState::MOUSE_LEFT:
                console.WriteLine("Left", 0x00FF00);
                break;
            case MouseState::MOUSE_RIGHT:
                console.WriteLine("Right", 0x0000FF);
                break;
            case MouseState::MOUSE_MIDDLE:
                console.WriteLine("Middle", 0xFFFF00);
                break;
            default:
                console.WriteLine("Unknown", 0xFF00FF);
                break;
        }
        console.WriteLine(strcat("FPS: ", to_string(fps)), 0xFAFAFA);
        console.WriteLine(strcat(ThreeStrCat(strcat("Eastern: ", to_string(time.hours)), strcat(":", strcat(to_string(time.minutes), strcat(":", to_string(time.seconds)))), " "), ""), rgb(170, 255, 170));
        console.WriteLine(strcat(ThreeStrCat(strcat("Central: ", to_string(Cent.hours)), strcat(":", strcat(to_string(Cent.minutes), strcat(":", to_string(Cent.seconds)))), " "), ""), rgb(170, 255, 170));
        graphics.put_image_alpha(get_mouse_x(), get_mouse_y(), Cursor);
        graphics.swap();
        console.Clear();
        frames++;
        //use rtc to get time in seconds getSeconds();
        if (getSeconds() != last)
        {
            last = getSeconds();
            fps = frames;
            frames = 0;
        }
        time = getTime(EasternTime);
        Cent = getTime(CenteralEurope);
        
    }
    ///
    /*Time_t time;
    time = getTime12Hour();
    char* dow = strcat("Day of Week: ",(char*)DAYOFWEEK(getDayOfWeek()));
    char* cd = strcat(strcat("Date: ", to_string(getMonth())), strcat("/", strcat(to_string(getDay()), strcat("/", strcat("20",to_string(getYear()))))));
    while (true)
    {
        console.WriteLine(strcat(ThreeStrCat(strcat("Time: ", to_string(time.hours)), strcat(":", strcat(to_string(time.minutes), strcat(":", to_string(time.seconds)))), " "), (time.pm == 1 ? "PM" : "AM")), rgb(170, 255, 170));
        console.WriteLine(cd, rgb(170, 255, 170));
        console.WriteLine(dow, rgb(170, 255, 170));
        graphics.swap();
        console.Clear();
        time = getTime12Hour();
    }*/
    

    halt();
}