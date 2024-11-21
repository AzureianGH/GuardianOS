#pragma once
#include <libhydrix/libhydrix.h>

typedef enum 
{
    WINDOWNone,
    WINDOWClose,
    WINDOWResize,
    WINDOWMove,
    WINDOWFocus,
    WINDOWUnfocus,
    WINDOWKeyPress,
    WINDOWKeyRelease,
    WINDOWMouseMove,
    WINDOWMousePress,
    WINDOWMouseRelease
} Aqua_Window_Event;

typedef struct
{
    KeyCode Key;
    bool IsPressed;
} Aqua_Keyboard_Event;

typedef void (*AquaWindowEventHandler)(Aqua_Window_Event, void*);
typedef void (*AquaWindowDrawHandler)(uint*);

typedef struct Aqua_Window
{
    uint64_t WinID;
    uint64_t X;
    uint64_t Y;
    uint64_t Width;
    uint64_t Height;
    char* Title;
    bool HasTitleBar;
    bool Focused;
    uint* Buffer;
    AquaWindowDrawHandler DrawHandler; // Function to draw the window
    AquaWindowEventHandler EventHandler; // Function to handle events

    // Constructor to initialize default values
    Aqua_Window()
    {
        WinID = 0;
        X = 0;
        Y = 0;
        Width = 0;
        Height = 0;
        Title = nullptr;
        HasTitleBar = true;
        Focused = false;
        Buffer = nullptr;
        DrawHandler = nullptr;
        EventHandler = nullptr;
    }

    // == operator
    bool operator==(const Aqua_Window& other) const
    {
        return X == other.X && Y == other.Y && Width == other.Width && Height == other.Height &&
               Title == other.Title && HasTitleBar == other.HasTitleBar && Focused == other.Focused &&
               Buffer == other.Buffer && DrawHandler == other.DrawHandler && EventHandler == other.EventHandler &&
               WinID == other.WinID;
    }
    bool operator!=(const Aqua_Window& other) const
    {
        return !(*this == other);
    }
    //bool operator != nullptr
    bool operator!=(const void* other) const
    {
        return this != other;
    }
} Aqua_Window;
