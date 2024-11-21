#include <libhydrix/hsyscall/syscall.h>
#include <libhydrix/hmouse/mouse.h>
#include <libhydrix/hkey/keyboard.h>
#include <libhydrix/hgl/graphics.h>


Console* syscall_console;

void InitializeSyscall(Console* con)
{
    syscall_console = con;
}
extern "C" int64_t ISRHandlerSyscall(registers_t *r)
{
    if (r->rax == 1)
    {
        if (r->rbx == 0)
        {
            syscall_console->ClearS();
            return 0;
        }
        else if (r->rbx == 1)
        {
            syscall_console->WriteS((char*)r->rcx, (int)r->rdx); //rcx is the address of the string, rdx is the color
            return 0;
        }
        else if (r->rbx == 2)
        {
            syscall_console->WriteLineS((char*)r->rcx, (int)r->rdx); //rcx is the integer, rdx is the color
            return 0;
        }
    }
    else if (r->rax == 2)
    {
        switch (r->rbx)
        {
        case 0:
            syscall_console->graphics->Display();
            return 0;
            break;
        case 1:
            syscall_console->graphics->Clear();
            return 0;
            break;
        case 2:
            if (r->rcx < syscall_console->graphics->Width && r->rdx < syscall_console->graphics->Height) {
                syscall_console->graphics->SwapBuffer[r->rdx * (syscall_console->graphics->Pitch / 4) + r->rcx] = r->rsi;
                return 0;
            }
            return -1;
            break;
        case 3:
            syscall_console->graphics->DrawLine(r->rcx, r->rdx, r->rsi, r->rdi, r->r8);
            return 0;
            break;
        case 4:
            syscall_console->graphics->DrawRectangle(r->rcx, r->rdx, r->rsi, r->rdi, r->r8);
            return 0;
            break;
        case 5:
            syscall_console->graphics->DrawFilledRectangle(r->rcx, r->rdx, r->rsi, r->rdi, r->r8);
            return 0;
            break;
        case 6:
            syscall_console->graphics->DrawCircle(r->rcx, r->rdx, r->rsi, r->rdi);
            return 0;
            break;
        case 7:
            syscall_console->graphics->DrawFilledCircle(r->rcx, r->rdx, r->rsi, r->rdi);
            return 0;
            break;
        case 8:
            syscall_console->graphics->DrawChar(r->rcx, r->rdx, r->rsi, r->rdi);
            return 0;
            break;
        case 9:
            syscall_console->graphics->DrawString((char*)r->rcx, r->rdx, r->rsi, r->rdi);
            return 0;
            break;
        case 10:
            BMPI Image;
            Image.width = r->rcx;
            Image.height = r->rdx;
            Image.data = (int*)r->rsi;
            syscall_console->graphics->DrawImage(r->r8, r->r9, Image);
            return 0;
            break;
        case 11:
            BMPA AImage;
            AImage.width = r->rcx;
            AImage.height = r->rdx;
            AImage.data = (long*)r->rsi;
            syscall_console->graphics->DrawAlphaImage(r->r8, r->r9, AImage);
            return 0;
            break;
        case 12:
            BMPI SImage;
            SImage.width = r->rcx;
            SImage.height = r->rdx;
            SImage.data = (int*)r->rsi;
            syscall_console->graphics->DrawStretchedImage(r->r8, r->r9, r->r10, r->r11, SImage);
            return 0;
            break;
        case 13:
            BMPA SAImage;
            SAImage.width = r->rcx;
            SAImage.height = r->rdx;
            SAImage.data = (long*)r->rsi;
            syscall_console->graphics->DrawStretchedAlphaImage(r->r8, r->r9, r->r10, r->r11, SAImage);
            return 0;
            break;
        
        }

    }
    
    else if (r->rax == 3)
    {
        switch (r->rbx)
        {
        case 0:
            return GetKeyDown((KeyCode)r->rcx);
            
            break;
        case 1:
            return KeyboardGetKey();
            break;
        case 2:
            KeyboardInit(syscall_console);
            return 0;
            break;
        case 3:
            DisableKeyboard();
            return 0;
            break;
        case 4:
            EnableKeyboard();
            return 0;
            break;
        }
    }
    else if (r->rax == 4)
    {
        if (r->rbx == 0)
        {
            return GetMouseXPos();
            
        }
        else if(r->rbx == 1)
        {
            return GetMouseYPos();
            
        }
        else if(r->rbx == 2)
        {
            return GetCurrentMouseState().State;
            
        }
        else if(r->rbx == 3)
        {
            MouseScroll scrollMC3 = GetMouseScroll();
            //first 8 bits are the state, next 8 bits are the delta
            return (scrollMC3.State << 8) | scrollMC3.Delta;
            
        }
        else if(r->rbx == 4)
        {
            SetMouseSensitivity((int)r->rcx);
            return 0;
        }
        
    }
    else if (r->rax == 5)
    {
        switch (r->rbx)
        {
        case 0:
            SetMouseConsole(syscall_console);
            return 0;
            break;
        case 1:
            MouseHandler(r);
            return 0;
            break;
        }
    }
    else if (r->rax == 6)
    {
        switch (r->rbx)
        {
        case 0:
            syscall_console->Clear();
            return 0;
            break;
        case 1:
            syscall_console->Write((char*)r->rcx, (int)r->rdx); //rcx is the address of the string, rdx is the color
            return 0;
            break;
        case 2:
            syscall_console->WriteLine((char*)r->rcx, (int)r->rdx); //rcx is the integer, rdx is the color
            return 0; // Success
            break;
        }
        return -1;
        
    }
    return -1; // Invalid syscall 
}