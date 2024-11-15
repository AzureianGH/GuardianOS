#pragma once
#include <libhydrix/hlow/cpu/cpu.h>

typedef struct
{
    registers_t regs;
    uint64_t pid;
    void (*Code)();
} Program_t;

Program_t CreateProgram(void (*Code)());
void AddProgram(Program_t program);
void SchedulerInit();
void StartNext();