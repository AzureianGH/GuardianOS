// DEBUG
#define SKIP_BOOT_FAILURE
#define SKIP_BOOT_WAIT
#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <libhydrix/libhydrix.h>
#include <basic.h>
#include <Taskbar/Taskbar.h>
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
char* ThreeStringConcatenate(char* one, char* two, char* three)
{
    return StringConcatenate(one, StringConcatenate(two, three));
}
void testproc()
{
    while (true)
    {
        console.WriteLine("Test Process", IColor::RGB(170, 255, 170));
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
    int* newdata = (int*)KernelCleanAllocate(BMP.width * BMP.height * display.bpp / 8);
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

    PortIO::OutByte(0x43, 0xB6);

    uint8_t l = static_cast<uint8_t>(divisor);
    uint8_t h = static_cast<uint8_t>(divisor >> 8);

    PortIO::OutByte(0x42, l);
    PortIO::OutByte(0x42, h);

    PortIO::OutByte(0x61, PortIO::InByte(0x61) | 0x3);
}

void stop()
{
    PortIO::OutByte(0x61, PortIO::InByte(0x61) & 0xFC);
}

void beep(uint64_t freq, uint64_t msec)
{

    play(freq);
    PITSleepMS(msec);
    stop();
}
void PrintAvailableResolutions(limine_framebuffer* framebuffer)
{
    uint64_t Count = framebuffer->mode_count;
    limine_video_mode** modes = framebuffer->modes;
    for (uint64_t i = 0; i < Count; i++)
    {
        limine_video_mode* mode = modes[i];
        console.WriteLine(ThreeStringConcatenate(StringConcatenate(ThreeStringConcatenate("Resolution: ", ToString(mode->width), "x"), ToString(mode->height)), "x", ToString(mode->bpp)), IColor::RGB(170, 255, 170));
        
    }
}

void DrawCursor()
{
    uint64_t mx = GetMouseXPos();
    uint64_t my = GetMouseYPos();
    //Border
    graphics.DrawLine(mx, my, mx, my + 15, 0xFFFFFF);
    graphics.DrawLine(mx, my, mx + 10, my + 10, 0xFFFFFF);
    graphics.DrawLine(mx + 10, my + 10, mx + 6, my + 10, 0xFFFFFF);
    graphics.DrawLine(mx + 6, my + 10, mx + 6, my + 12, 0xFFFFFF);
    graphics.DrawLine(mx + 7, my + 13, mx + 7, my + 15, 0xFFFFFF);
    graphics.DrawLine(mx + 7, my + 15, mx + 5, my + 15, 0xFFFFFF);
    graphics.DrawLine(mx + 5, my + 15, mx + 5, my + 14, 0xFFFFFF);
    graphics.DrawLine(mx + 4, my + 14, mx + 4, my + 13, 0xFFFFFF);
    graphics.DrawLine(mx + 3, my + 12, mx, my + 15, 0xFFFFFF);
    //Insides
    graphics.DrawLine(mx + 1, my + 2, mx + 1, my + 13, 0x0f141c);
    graphics.DrawLine(mx + 2, my + 3, mx + 2, my + 12, 0x0f141c);
    graphics.DrawLine(mx + 3, my + 4, mx + 3, my + 11, 0x0f141c);
    graphics.DrawLine(mx + 4, my + 5, mx + 4, my + 12, 0x0f141c);
    graphics.DrawLine(mx + 5, my + 6, mx + 5, my + 13, 0x0f141c);
    graphics.DrawLine(mx + 6, my + 7, mx + 6, my + 9, 0x0f141c);
    graphics.DrawPixel(mx + 7, my + 8, 0x0f141c);
    graphics.DrawPixel(mx + 7, my + 9, 0x0f141c);
    graphics.DrawPixel(mx + 8, my + 9, 0x0f141c);
    graphics.DrawLine(mx + 6, my + 13, mx + 6, my + 14, 0x0f141c);
}

void DrawPixelWithAlpha(int x, int y, int color, float alpha) {
        // Assuming you have a function to draw a pixel with alpha blending
        int r = (color >> 16) & 0xFF;
        int g = (color >> 8) & 0xFF;
        int b = color & 0xFF;

        // Blend the color with the existing pixel color here
        // Example:
        int existingColor = graphics.GetPixel(x, y);
        int existingR = (existingColor >> 16) & 0xFF;
        int existingG = (existingColor >> 8) & 0xFF;
        int existingB = existingColor & 0xFF;
        int newR = (int)((r * alpha) + (existingR * (1 - alpha)));
        int newG = (int)((g * alpha) + (existingG * (1 - alpha)));
        int newB = (int)((b * alpha) + (existingB * (1 - alpha)));
        graphics.DrawPixel(x, y, (newR << 16) | (newG << 8) | newB);
};
extern void kernel_main() {
    
    
    int InitFailuresIndex = 0;
    bool InitalizedFailure = false;
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
    size_t memsize = RetrieveTotalMemory(memmap);
    display.width = framebuffer->width;
    display.height = framebuffer->height;
    display.bpp = framebuffer->bpp;
    display.address = framebuffer->address;
    //one after the framebuffer
    //heap_init the offset of hhdm
    InitializeHeap(offsetofhhdm);
    string* InitFailures = new string[100];
    graphics.Init((uint32_t*)framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch, framebuffer->bpp, framebuffer->red_mask_shift, framebuffer->green_mask_shift, framebuffer->blue_mask_shift, framebuffer->red_mask_size, framebuffer->green_mask_size, framebuffer->blue_mask_size);
    
    
    console.Init(&graphics, 16, false);
    
    graphics.Clear();
    BMPI tridentstartup;
    tridentstartup.data = (int*)tridentfull;
    tridentstartup.height = TRIDENTFULL_HEIGHT;
    tridentstartup.width = TRIDENTFULL_WIDTH;
    console.Clear();
    graphics.DrawImage((display.width / 2) - 250, (display.height / 2) - 250, tridentstartup);
    //if type is 0, add to memsize
    
    SetISRConsole(&console);
    KeyboardInit(&console);
    SetMouseConsole(&console);
    InitializeSyscall(&console);
    console.WriteLine("Initializing FPU...", IColor::RGB(170, 170, 170));
    if (FPU::IsFPUEnabled())
    {
        console.WriteLine("FPU Enabled!", IColor::RGB(170, 255, 170));
    }
    else
    {
        console.WriteLine("FPU Not Detected!", IColor::RGB(255, 170, 170));
        console.WriteLine("FPU Failed To Initialize!", IColor::RGB(255, 120, 120));
        InitalizedFailure = true;
        InitFailures[InitFailuresIndex] = "FPU";
        InitFailuresIndex++;
    }
    console.WriteLine("Initializing RNG...", IColor::RGB(170, 170, 170));
    rand_seed = (long int)framebuffer;
    console.WriteLine("RNG Initialized!", IColor::RGB(170, 255, 170));
    //gdt
    console.WriteLine("Initializing GDT...", IColor::RGB(170, 170, 170));
    
    InitializeGDT();
    console.WriteLine("GDT Initialized!", IColor::RGB(170, 255, 170));
    //idt
    console.WriteLine("Initializing IDT...", IColor::RGB(170, 170, 170));
    InitializeISR();
    
    EnableInterrupts();
    SetPITFrequency(1000);
    InitializeTime();
    console.WriteLine("IDT Initialized!", IColor::RGB(170, 255, 170));
    //init PCI
    console.WriteLine("Initializing PCI...", IColor::RGB(170, 170, 170));
    PCIInit();
    console.WriteLine("PCI Initialized!", IColor::RGB(170, 255, 170));
    //paging
    #ifndef SKIP_BOOT_FAILURE
        if (InitalizedFailure)
        {
            
            for (int i = 0; i < InitFailuresIndex; i++)
            {
                console.WriteLine(StringConcatenate("Failed To Initialize: ", InitFailures[i]), IColor::RGB(255, 170, 170));
            }
            console.WriteLine(ThreeStringConcatenate("FAILURE GUARDIANOS VERSION: ", OS_Version_, ""), IColor::RGB(255, 170, 170));
            console.WriteLine("Initialization Failed!", IColor::RGB(255, 100, 100));
            graphics.DrawString("SYSTEM HALTED", (graphics.Width / 2) - 210, graphics.Height - 100, 4, 0xFF0000);
            graphics.Display();
            hcf();
        }
    #endif
    console.WriteLine("Initialization Complete!", IColor::RGB(170, 255, 170));
    console.WriteLine(ThreeStringConcatenate("[GuardianOS Version: ", OS_Version_, "]"), IColor::RGB(170, 170, 255));
    graphics.Display();
    #ifndef SKIP_BOOT_WAIT
        RudamentaryWait(4000);
    #endif

    graphics.Clear();
    console.ClearS();
    console.WriteLine("Welcome to GuardianOS!", IColor::RGB(170, 255, 170));
    //print resolution, use StringConcatenate and threecat
    console.WriteLine(ThreeStringConcatenate(ThreeStringConcatenate("Resolution: ", ToString(display.width), StringConcatenate("x", ToString(display.height))), "x", ToString(framebuffer->bpp)), IColor::RGB(170, 255, 170));
    //print all framebuffers
    console.WriteLine(StringConcatenate("Framebuffer Count: ", ToString(framebuffer_request.response->framebuffer_count)), IColor::RGB(170, 255, 170));
    //print memory size in MB
    console.WriteLine(ThreeStringConcatenate("Memory Size: ", ToString(memsize / 1024 / 1024), " MB"), IColor::RGB(170, 255, 170));
    PrintAvailableResolutions(framebuffer);
    //capture
    graphics.Display();
    #ifndef SKIP_BOOT_WAIT
        RudamentaryWait(4000);
    #endif
    graphics.Clear();
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
    
    char* memesizzze = ToString(memsize);
    BMPI Stretched = *StretchImage(&BGImg, display.width, display.height);

    /// #########
    /// # START #
    /// #########
    Taskbar taskbar(&graphics);
    graphics.SetHz(200);
    Vector<pci_device_t> Testing = PCIGetDevices();
    Vector<StringObj> PCIStrings;
    for (int i = 0; i < Testing.Length(); i++)
    {
        pci_device_t dev = Testing.At(i);
        StringObj devs = (StringObj)"Device: " + (StringObj)PCIDevice2IDString(dev);
        PCIStrings.PushBack(devs);
    }
    while (true)
    {
        console.Clear();
        taskbar.Draw();
        console.WriteLine("");
        console.WriteLine("");
        console.WriteLine("");
        //write each device String
        for (int i = 0; i < PCIStrings.Length(); i++)
        {
            console.WriteLine(PCIStrings.At(i).c_str(), IColor::RGB(170, 255, 170));
        }
        

        DrawCursor();
        graphics.Display();
    }
    halt();
}