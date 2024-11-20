#include <libhydrix/hkey/keyboard.h>
#include <libhydrix/hio/io.h>

Console* Keyboard_Console_IDT;
char LastScancode = 0;
//scancodemap
char scancodemap[58] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};
char upperscancode[58] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0, 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '
};
bool GetKeyDown(KeyCode scancode)
{
    return (LastScancode == (char)scancode);
}
inline uint8_t getScancode()
{
    return PortIO::InByte(0x60);
}
bool AllowKeyboard = true;

void DisableKeyboard()
{
    AllowKeyboard = false;
}
void EnableKeyboard()
{
    AllowKeyboard = true;
}
Vector<void(*)(KeyboardData_t)> InterruptsToCall;
void AddKeyboardInterrupt(void (*func)(KeyboardData_t))
{
    InterruptsToCall.PushBack(func);
}
void RemoveKeyboardInterrupt(void (*func)(KeyboardData_t))
{
    for (int i = 0; i < InterruptsToCall.Length(); i++)
    {
        if (InterruptsToCall[i] == func)
        {
            InterruptsToCall.Erase(i);
            return;
        }
    }
}
void KeyboardHandler(registers_t *r)
{
    if (!AllowKeyboard)
    {
        return;
    }
    uint8_t scancode = getScancode();
    //run handlers
    KeyboardData_t data;
    data.key = (KeyCode)scancode;
    data.character = scancodemap[scancode];
    //check if key is pressed
    if (scancode & 0x80)
    {
        data.pressed = false;
    }
    else
    {
        data.pressed = true;
    }
    for (int i = 0; i < InterruptsToCall.Length(); i++)
    {
        InterruptsToCall[i](data);
    }
    LastScancode = scancode;
    return;
}

void KeyboardInit(Console* console)
{
    Keyboard_Console_IDT = console;
    InterruptsToCall = Vector<void(*)(KeyboardData_t)>();
}

uint64_t KeyboardGetKey()
{
    return (uint64_t)LastScancode;
}
