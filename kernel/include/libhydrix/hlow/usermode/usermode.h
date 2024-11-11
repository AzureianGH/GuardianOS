#pragma once
#include <libhydrix/libhydrix.h>

//function to set a new usermode function to call
void SetUsermodeEntryFunction(void (*func)());

void StartUserMode();