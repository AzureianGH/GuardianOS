#pragma once
#include <stdint.h>
#include <stddef.h>
#include <libhydrix/hlow/pci/pci.h>

struct IDEChannelRegisters {
   unsigned short base;  // I/O Base.
   unsigned short ctrl;  // Control Base
   unsigned short bmide; // Bus Master IDE
   unsigned char  nIEN;  // nIEN (No Interrupt);
};

void SetIDEConsole(Console* console);
void IDEInit(unsigned int BAR0, unsigned int BAR1, unsigned int BAR2, unsigned int BAR3, unsigned int BAR4);