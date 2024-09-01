#pragma once
#include <stddef.h>
#include <libhydrix/hstring/string.h>
#include <libhydrix/hio/io.h>
#include <libhydrix/sdefs.h>
#include <libhydrix/hmem/smem/smem.h>
#include <libhydrix/hcon/console.h>
#include <libhydrix/hlow/pit/pit.h>
void SetAPCIConsole(Console* console_);
int acpiEnable(void);
int initAcpi(void);
string GetReasonForFailureACPI();