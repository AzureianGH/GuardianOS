#include <basic.h>
#include <libhydrix/libhydrix.h>

void DrawMouseToolTip(Graphics* GraphicsObject, int x, int y, cstring text)
{
    int FontGlyphWidth = GraphicsObject->GlyphWidth / 2; // 8
    int textwidth = FontGlyphWidth * StringLength(text); // 8 * length of text 
    GraphicsObject->DrawFilledRectangle(x + 16, y + 16, textwidth + 10, 20, 0x000000);
    GraphicsObject->DrawRectangle(x + 16, y + 16, textwidth + 10, 20, 0xFFFFFF);
    GraphicsObject->DrawString((string)text, x + 19, y + 19, 0xFFFFFF);
}