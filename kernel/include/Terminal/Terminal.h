#pragma once

#include <stdint.h>
#include <stddef.h>
#include <libhydrix/libhydrix.h>

class Terminal
{
    private:
        /// VARIABLES
        char* buffer;
        size_t buffer_size;
        size_t buffer_index;
        Graphics* graphics;
        bool cursor_enabled = true; // Default enabled
        bool cursor_blink = true; // Default blink
        uint cursor_x = 0; // Default position
        uint cursor_y = 0; // Default position
        uint cursor_width = 2; // Default width
        uint cursor_height = 16; // Default height
        uint cursor_color = 0xFFFFFF; // White
        uint cursor_blink_rate = 500; // 500 ms
        bool cursor_visible = true;
        ulong drawing_width;
        ulong drawing_height;
        ulong FittableCharsPerLine;
        ulong FittableLines;
        uint char_width;
        uint char_height;
        uint char_spacing;
        uint char_color = 0xFFFFFF;
        uint background_color;
        uint cursor_blink_timer = 0;
        bool locked = false; // Default unlocked
        size_t last_buffer_index = 0;
        bool redraw_needed = false;

        /// FUNCTIONS
        void KeyboardTypingHandler(KeyboardData_t data);
        static void StaticKeyboardTypingHandler(KeyboardData_t data);
        void DrawCursor();
        void RedrawRegion(uint x_start, uint y_start, uint width, uint height);
    public:
        void Init(Graphics* graphics);
        void SetCursorEnabled(bool enabled);
        void SetCursorBlink(bool blink);
        void SetCursorPos(uint x, uint y);
        void SetCursorSize(uint width, uint height);
        void SetCursorColor(uint color);
        void SetCursorBlinkRate(uint rate);
        void SetCharWidth(uint width);
        void SetCharHeight(uint height);
        void SetCharSpacing(uint spacing);
        void SetCharColor(uint color);
        void SetBackgroundColor(uint color);
        void Clear();
        void DrawChar(char c, uint x, uint y);
        void DrawString(const char* str, uint x, uint y);
        void DrawString(StringObj str, uint x, uint y);
        void Lock();  
        void Unlock();
        void Recalculate();
        void Display();
};