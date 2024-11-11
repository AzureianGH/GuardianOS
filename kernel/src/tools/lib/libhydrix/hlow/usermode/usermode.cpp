#include <libhydrix/hlow/usermode/usermode.h>
extern "C" void begin_usermode();
void StartUserMode()
{
    begin_usermode();
}

void defaultUsermodeEntryFunction()
{
    //draw to screen
    int test = 0;
    char* test_string = "Hello!!!";
    int colortest = 0xFFFFFF;
    while (true)
    {
        if (test > 100)
        {
            test = 0;
        }
    }
}

void (*UsermodeEntryFunction)() = defaultUsermodeEntryFunction;
void SetUsermodeEntryFunction(void (*func)())
{
    UsermodeEntryFunction = func;
}


extern "C" void _InvokableUsermodeEntry()
{
    asm volatile("push %rax");
    asm volatile("push %rbx");
    asm volatile("mov $0x1, %rax");
    asm volatile("mov $0x0, %rbx");
    asm volatile("syscall");
    asm volatile("pop %rbx");
    asm volatile("pop %rax");

    while(1);
}

