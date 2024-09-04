#pragma once
#include <stdint.h>
#include <stddef.h>
typedef struct {
	union {
		uint32_t ax;
		uint32_t magic;
	};
	union {
		uint32_t bx;
		size_t size;
	};
	union {
		uint32_t cx;
		uint16_t command;
	};
	union {
		uint32_t dx;
		uint16_t port;
	};
	uint32_t si;
	uint32_t di;
} vmware_cmd;
