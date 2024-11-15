#include <libhydrix/hlow/prog/program.h>
#include <libhydrix/libhydrix.h>

// The currently running program's state
Program_t CurrentProgram = {0};

// Queue of programs waiting to be scheduled
Vector<Program_t> WaitingPrograms;

Program_t CreateProgram(void (*Code)())
{
    Program_t program = {0};
    program.Code = Code;
    program.pid = WaitingPrograms.Length();
    return program;
}

void AddProgram(Program_t program)
{
    WaitingPrograms.PushBack(program);
}
// Initializes the scheduler
void SchedulerInit()
{
    WaitingPrograms = Vector<Program_t>();
}

// Starts the next program by updating the CPU registers
void StartNext()
{
    // If there are programs waiting in the queue
    if (!WaitingPrograms.IsEmpty())
    {
        // Save the current program's state if it's valid
        if (CurrentProgram.regs.rsp != 0)
        {
            WaitingPrograms.PushBack(CurrentProgram);
        }
        if (WaitingPrograms.IsEmpty()) {
            return; // No program to schedule
        }

        // Switch to the next program in the queue
        CurrentProgram = WaitingPrograms.Front();
        WaitingPrograms.Erase(0);

        // Load the state of the new program into the CPU registers
        asm volatile(
            "mov %0, %%r15\n"
            "mov %1, %%r14\n"
            "mov %2, %%r13\n"
            "mov %3, %%r12\n"
            "mov %4, %%r11\n"
            "mov %5, %%r10\n"
            "mov %6, %%r9\n"
            "mov %7, %%r8\n"
            "mov %8, %%rsi\n"
            "mov %9, %%rdi\n"
            "mov %10, %%rbp\n"
            "mov %11, %%rdx\n"
            "mov %12, %%rcx\n"
            "mov %13, %%rbx\n"
            "mov %14, %%rax\n"
            "mov %15, %%rsp\n"
            "push %20\n"         // Push new stack segment (SS)
            "push %19\n"         // Push new stack pointer (RSP)
            "push %18\n"         // Push flags (RFLAGS)
            "push %17\n"         // Push code segment (CS)
            "push %16\n"         // Push new instruction pointer (RIP)
            "iretq\n"            // Return to new program context
            :
            : "g"(CurrentProgram.regs.r15), "g"(CurrentProgram.regs.r14),
              "g"(CurrentProgram.regs.r13), "g"(CurrentProgram.regs.r12),
              "g"(CurrentProgram.regs.r11), "g"(CurrentProgram.regs.r10),
              "g"(CurrentProgram.regs.r9),  "g"(CurrentProgram.regs.r8),
              "g"(CurrentProgram.regs.rsi), "g"(CurrentProgram.regs.rdi),
              "g"(CurrentProgram.regs.rbp), "g"(CurrentProgram.regs.rdx),
              "g"(CurrentProgram.regs.rcx), "g"(CurrentProgram.regs.rbx),
              "g"(CurrentProgram.regs.rax), "g"(CurrentProgram.regs.rsp),
              "g"(CurrentProgram.regs.rip), "g"(CurrentProgram.regs.cs),
              "g"(CurrentProgram.regs.rflags), "g"(CurrentProgram.regs.rsp),
              "g"(CurrentProgram.regs.ss)
            : "memory"
        );
    }
}

// Scheduler interrupt handler
extern void SchedulerInterruptHandler(registers_t* reg)
{
    // Save the current program's state
    CurrentProgram.regs = *reg;

    // Switch to the next program
    StartNext();
}
