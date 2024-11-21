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
    for (uint y = y_start; y < y_start + height; y++)
    {
        for (uint x = x_start; x < x_start + width; x++)
        {
            size_t index = y * FittableCharsPerLine + x;
            if (index < buffer_size)
            {
                DrawChar(buffer[index], x, y);
            }
        }
    }
}

void Terminal::DrawCursor()
{
    if (cursor_enabled && cursor_visible)
    {
        // Draw the cursor directly on the graphics layer
        graphics->DrawFilledRectangle(
            cursor_x * (char_width + char_spacing),
            cursor_y * char_height,
            cursor_width,
            cursor_height,
            cursor_color
        );
    }
}

void Terminal::KeyboardTypingHandler(KeyboardData_t data)
{
    if (locked || !data.pressed || !buffer)
        return;

    // Update buffer with new character and redraw affected region
    buffer[buffer_index] = data.character;
    uint x = buffer_index % FittableCharsPerLine;
    uint y = buffer_index / FittableCharsPerLine;
    buffer_index = (buffer_index + 1) % buffer_size;

    RedrawRegion(x, y, 1, 1);
    cursor_x = buffer_index % FittableCharsPerLine;
    cursor_y = buffer_index / FittableCharsPerLine;
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
    // Update dimensions and allocate buffer
    FittableCharsPerLine = drawing_width / (char_width + char_spacing);
    FittableLines = drawing_height / char_height;
    buffer_size = FittableCharsPerLine * FittableLines;

    if (buffer)
        KernelFree(buffer);
    buffer = (char*)KernelAllocate(buffer_size);
    buffer_index = 0;
    Clear();
}

void Terminal::Display()
{
    Clear();
    RedrawRegion(0, 0, FittableCharsPerLine, FittableLines);
    DrawCursor();
}
