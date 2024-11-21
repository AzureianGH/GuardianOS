#include <Terminal/Terminal.h>
static Terminal* instance;

void Terminal::SetCursorEnabled(bool enabled)
{
    cursor_enabled = enabled;
}

void Terminal::SetCursorBlink(bool blink)
{
    cursor_blink = blink;
}

void Terminal::SetCursorPos(uint x, uint y)
{
    cursor_x = x;
    cursor_y = y;
}

void Terminal::SetCursorSize(uint width, uint height)
{
    cursor_width = width;
    cursor_height = height;
}

void Terminal::SetCursorColor(uint color)
{
    cursor_color = color;
}

void Terminal::SetCursorBlinkRate(uint rate)
{
    cursor_blink_rate = rate;
}

void Terminal::SetCharWidth(uint width)
{
    char_width = width;
}

void Terminal::SetCharHeight(uint height)
{
    char_height = height;
}

void Terminal::SetCharSpacing(uint spacing)
{
    char_spacing = spacing;
}

void Terminal::SetCharColor(uint color)
{
    char_color = color;
}

void Terminal::SetBackgroundColor(uint color)
{
    background_color = color;
}

void Terminal::Clear()
{
    // Reset the buffer and redraw the background
    memset(buffer, ' ', buffer_size);
    redraw_needed = true;
}

void Terminal::DrawChar(char c, uint x, uint y)
{
    // Draw a character directly on the graphics layer
    graphics->DrawChar(
        c,
        x * (char_width + char_spacing),
        y * char_height,
        char_color
    );
}

void Terminal::RedrawRegion(uint x_start, uint y_start, uint width, uint height)
{

}

void Terminal::DrawCursor()
{

}

void Terminal::KeyboardTypingHandler(KeyboardData_t data)
{

}

void Terminal::StaticKeyboardTypingHandler(KeyboardData_t data)
{
    if (instance)
        instance->KeyboardTypingHandler(data);
}

void Terminal::Init(Graphics* graphics)
{
    this->graphics = graphics;
    instance = this;

    // Initialize terminal properties
    drawing_width = graphics->Width;
    drawing_height = graphics->Height;
    char_width = graphics->GlyphWidth;
    char_height = graphics->GlyphHeight;
    char_spacing = graphics->FontLetterSpacing;
    char_color = 0xFFFFFF;
    background_color = 0x000000;

    Recalculate();
    AddKeyboardInterrupt(StaticKeyboardTypingHandler);
    Clear();
}

void Terminal::Recalculate()
{
    // Recalculate the number of characters that can fit on the screen
    FittableCharsPerLine = drawing_width / (char_width + char_spacing);
    FittableLines = drawing_height / char_height;
    buffer_size = FittableCharsPerLine * FittableLines;
    buffer = (char*)KernelAllocate(buffer_size);
    Clear();
}

void Terminal::Display()
{
    Clear();
    RedrawRegion(0, 0, FittableCharsPerLine, FittableLines);
    DrawCursor();
}
