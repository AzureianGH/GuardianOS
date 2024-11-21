#pragma once
#include <libhydrix/hcon/console.h>
#include <stdint.h>
#include <libhydrix/hlow/cpu/cpu.h>
#define MOUSE_IRQ 12
#define MOUSE_PORT 0x60
#define MOUSE_COMMAND 0x64

typedef enum
{

    MOUSE_NONE = 0,
    MOUSE_LEFT = 1,
    MOUSE_RIGHT = 2,
    MOUSE_MIDDLE = 3,
} MouseState_E;

typedef struct MouseState
{
    MouseState_E State;
    uint8_t X;
    uint8_t Y;
} MouseState;


typedef enum MouseScrollState
{
    MOUSE_SCROLL_UP = 4,
    MOUSE_SCROLL_DOWN = 5,
} MouseScrollState;

typedef struct MouseScroll
{
    MouseScrollState State;
    uint8_t Delta;
} MouseScroll;



MouseScroll GetMouseScroll();
MouseState GetCurrentMouseState();
void SetMouseConsole(Console *console);
void MouseHandler(registers_t *r);
void SetMouseSensitivity(int new_sensitivity);
void AddMouseInterrupt(void (*func)(MouseState));
void RemoveMouseInterrupt(void (*func)(MouseState));

int32_t GetMouseXPos();
int32_t GetMouseYPos();