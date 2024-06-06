#include "graphics.h"
#include <stdint.h>
#include <stddef.h>
//graphics::graphics
Graphics::Graphics() {
}

void Graphics::init(limine_framebuffer fb) {
    framebuffer = fb;
    //Set the swap buffer to the framebuffer address + the pitch * height
    SwapBuffer = (uint32_t*)framebuffer.address + (framebuffer.width * framebuffer.height);
}
void Graphics::put_pixel(int x, int y, int color) {
    volatile uint32_t *fb_ptr = static_cast<volatile uint32_t *>(SwapBuffer);
    fb_ptr[y * (framebuffer.pitch / 4) + x] = color;
}
void Graphics::fill_screen(int color) {
    for (int y = 0; y < framebuffer.height; y++) {
        for (int x = 0; x < framebuffer.width; x++) {
            put_pixel(x, y, color);
        }
    }
}
int GetOffsetFromText(string str)
{
    //get pixel offset per char and add
    int len = strlen(str);
    return len * 10;
}
void Graphics::put_string(char* str, int x, int y, int color) {
    //draw char but if space move x by 10
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == ' ') {
            x += 1;
        }
        x += 4;
        put_char(str[i], x + i * 10, y, color);
        //2 px padding
    }
}
void Graphics::clear() {
    fill_screen(0);
}
void Graphics::put_char(char c, int x, int y, int color) {
            if (c == 'a' || c == 'A') {
                //bottom left to top left
                put_line(x, y + 20, x, y, color);
                //bottom right to top right
                put_line(x + 10, y + 20, x + 10, y, color);
                //top left to top right
                put_line(x, y, x + 10, y, color);
                //middle left to middle right
                put_line(x, y + 10, x + 10, y + 10, color);
            }
            //b
            else if (c == 'b' || c == 'B') {
                //top left to bottom left
                put_line(x, y, x, y + 20, color);
                //bottom left to bottom right
                put_line(x, y + 20, x + 10, y + 20, color);
                //top left to top right
                put_line(x, y, x + 10, y, color);
                //middle left to middle of character
                put_line(x, y + 10, x + 7, y + 10, color);
                // bottom right to middle right
                put_line(x + 10, y + 20, x + 10, y + 13, color);
                // top right to middle right
                put_line(x + 10, y, x + 10, y + 7, color);
                //from y 7 to middle of char
                put_line(x + 10, y + 7, x + 7, y + 10, color);
                // from y 13 to middle of char
                put_line(x + 10, y + 13, x + 7, y + 10, color);

            }
            //c
            else if (c == 'c' || c == 'C') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
            }
            //d
            else if (c == 'd' || c == 'D') {
                //Draw top left to middle top
                put_line(x, y, x + 5, y, color);
                //Draw top left to bottom left
                put_line(x, y, x, y + 20, color);
                //Draw bottom left to middle bottom
                put_line(x, y + 20, x + 5, y + 20, color);
                // middle bottom to middle right, diagonal
                put_line(x + 5, y + 20, x + 10, y + 15, color);
                //middle top to middle right, diagonal
                put_line(x + 5, y, x + 10, y + 5, color);
                //line from y + 5 to y + 15
                put_line(x + 10, y + 5, x + 10, y + 15, color);

                


            }
            //e
            else if (c == 'e' || c == 'E') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 20, color);
                put_line(x, y + 10, x + 10, y + 10, color);
                put_line(x, y + 20, x + 10, y + 20, color);
            }
            //f
            else if (c == 'f' || c == 'F') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 20, color);
                put_line(x, y + 10, x + 10, y + 10, color);
            }
            //g
            else if (c == 'g' || c == 'G') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
                put_line(x + 10, y + 10, x + 10, y + 20, color);
                put_line(x + 5, y + 20, x + 10, y + 20, color);
            }
            //h
            else if (c == 'h' || c == 'H') {
                put_line(x, y, x, y + 20, color);
                put_line(x + 10, y, x + 10, y + 20, color);
                put_line(x, y + 10, x + 10, y + 10, color);
            }
            //i
            else if (c == 'i' || c == 'I') {
                put_line(x + 5, y, x + 5, y + 20, color);
                put_line(x, y, x + 10, y, color);
                put_line(x, y + 20, x + 10, y + 20, color);
            }
            //j
            else if (c == 'j' || c == 'J') {
                //draw line from right top to bottom right
                put_line(x + 10, y, x + 10, y + 20, color);
                //draw line from bottom left to bottom right
                put_line(x, y + 20, x + 10, y + 20, color);
                //from bottom left to middle left
                put_line(x, y + 20, x, y + 10, color);
                //from top left to top right
                put_line(x, y, x + 10, y, color);
            }
            //k
            else if (c == 'k' || c == 'K') {
                put_line(x, y, x, y + 20, color);
                put_line(x, y + 10, x + 10, y, color);
                put_line(x, y + 10, x + 10, y + 20, color);
            }
            //l
            else if (c == 'l' || c == 'L') {
                put_line(x, y, x, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
            }
            //m
            else if (c == 'm' || c == 'M') {
                put_line(x, y, x, y + 20, color);
                put_line(x + 10, y, x + 10, y + 20, color);
                put_line(x, y, x + 5, y + 10, color);
                put_line(x + 5, y + 10, x + 10, y, color);
            }
            //n
            else if (c == 'n' || c == 'N') {
                put_line(x, y, x, y + 20, color);
                put_line(x + 10, y, x + 10, y + 20, color);
                put_line(x, y, x + 10, y + 20, color);
            }
            //o
            else if (c == 'o' || c == 'O') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
                put_line(x + 10, y, x + 10, y + 20, color);
            }
            //p
            else if (c == 'p' || c == 'P') {
                put_line(x, y, x, y + 20, color);
                put_line(x, y, x + 10, y, color);
                put_line(x, y + 10, x + 10, y + 10, color);
                put_line(x + 10, y, x + 10, y + 10, color);
            }
            //q
            else if (c == 'q' || c == 'Q') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
                put_line(x + 10, y, x + 10, y + 20, color);
                put_line(x + 5, y + 10, x + 10, y + 20, color);
            }
            //r
            else if (c == 'r' || c == 'R') {
                put_line(x, y, x, y + 20, color);
                put_line(x, y, x + 10, y, color);
                put_line(x, y + 10, x + 10, y + 10, color);
                put_line(x + 10, y, x + 10, y + 10, color);
                put_line(x, y + 10, x + 10, y + 20, color);
            }
            //s
            else if (c == 's' || c == 'S') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 10, color);
                put_line(x, y + 10, x + 10, y + 10, color);
                put_line(x + 10, y + 10, x + 10, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
            }
            //t
            else if (c == 't' || c == 'T') {
                put_line(x, y, x + 10, y, color);
                put_line(x + 5, y, x + 5, y + 20, color);
            }
            //u
            else if (c == 'u' || c == 'U') {
                put_line(x, y, x, y + 20, color);
                put_line(x + 10, y, x + 10, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
            }
            //v
            else if (c == 'v' || c == 'V') {
                put_line(x, y, x + 5, y + 20, color);
                put_line(x + 10, y, x + 5, y + 20, color);
            }
            //w
            else if (c == 'w' || c == 'W') {
                //top left to bottom left
                put_line(x, y, x, y + 20, color);
                //top right to bottom right
                put_line(x + 10, y, x + 10, y + 20, color);
                // bottom left to middle
                put_line(x, y + 20, x + 5, y + 10, color);
                //bottom right to middle
                put_line(x + 10, y + 20, x + 5, y + 10, color);

            }
            //x
            else if (c == 'x' || c == 'X') {
                put_line(x, y, x + 10, y + 20, color);
                put_line(x + 10, y, x, y + 20, color);
            }
            //y
            else if (c == 'y' || c == 'Y') {
                put_line(x, y, x + 5, y + 10, color);
                put_line(x + 10, y, x + 5, y + 10, color);
                put_line(x + 5, y + 10, x + 5, y + 20, color);
            }
            //z
            else if (c == 'z' || c == 'Z') {
                put_line(x, y, x + 10, y, color);
                put_line(x + 10, y, x, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
            }
            //space
            else if (c == ' ') {
                return;
            } 
            //0
            else if (c == '0') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
                put_line(x + 10, y, x + 10, y + 20, color);
                //line from bottom left to top right
                put_line(x, y + 20, x + 10, y, color);
            }
            //1
            else if (c == '1') {
                put_line(x + 5, y, x + 5, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
                //diag
                put_line(x + 5, y, x, y + 5, color);
            }
            //2
            else if (c == '2') {
                put_line(x, y, x + 10, y, color);
                put_line(x + 10, y, x + 10, y + 10, color);
                put_line(x, y + 10, x + 10, y + 10, color);
                put_line(x, y + 10, x, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
            }
            //3
            else if (c == '3') {
                put_line(x, y, x + 10, y, color);
                put_line(x + 10, y, x + 10, y + 20, color);
                put_line(x, y + 10, x + 10, y + 10, color);
                put_line(x, y + 20, x + 10, y + 20, color);
            }
            //4
            else if (c == '4') {
                put_line(x, y, x, y + 10, color);
                put_line(x, y + 10, x + 10, y + 10, color);
                put_line(x + 10, y, x + 10, y + 20, color);
            }
            //5
            else if (c == '5') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 10, color);
                put_line(x, y + 10, x + 10, y + 10, color);
                put_line(x + 10, y + 10, x + 10, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
            }
            //6
            else if (c == '6') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 20, color);
                put_line(x, y + 10, x + 10, y + 10, color);
                put_line(x + 10, y + 10, x + 10, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
            }
            //7
            else if (c == '7') {
                put_line(x, y, x + 10, y, color);
                put_line(x + 10, y, x + 10, y + 20, color);
            }
            //8
            else if (c == '8') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 20, color);
                put_line(x, y + 10, x + 10, y + 10, color);
                put_line(x + 10, y + 10, x + 10, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
                //line from top right to bottom right
                put_line(x + 10, y, x + 10, y + 20, color);
            }
            //9
            else if (c == '9') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 10, color);
                put_line(x, y + 10, x + 10, y + 10, color);
                put_line(x + 10, y + 10, x + 10, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
                //line from top right to middle right
                put_line(x + 10, y, x + 10, y + 10, color);
            }
            //.
            else if (c == '.') {
                put_filled_rect(x + 4, y + 18, 3, 3, color);
            }
            //,
            else if (c == ',') {
                put_filled_rect(x + 4, y + 18, 1, 3, color);
                // pixel under the bottom left of filled rect
                put_filled_rect(x + 3, y + 21, 1, 2, color);
                put_filled_rect(x + 2, y + 22, 1, 2, color);


            }
            //!
            else if (c == '!') {
                put_filled_rect(x + 4, y + 18, 1, 4, color);
                put_filled_rect(x + 4, y, 1, 14, color);
            }
            //?
            else if (c == '?') {
                put_line(x, y, x + 10, y, color);
                put_line(x + 10, y, x + 10, y + 10, color);
                put_line(x, y + 10, x + 10, y + 10, color);
                put_line(x + 10, y + 10, x + 10, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
                put_line(x + 5, y + 20, x + 5, y + 22, color);
                
            }
            //:
            else if (c == ':') {
                put_filled_rect(x + 4, y + 4, 2, 2, color);
                put_filled_rect(x + 4, y + 16, 2, 2, color);
            }
            //;
            else if (c == ';') {
                put_filled_rect(x + 4, y + 8, 2, 2, color);
                put_filled_rect(x + 4, y + 12, 2, 2, color);
                put_line(x + 5, y + 14, x + 5, y + 16, color);
            }
            //'
            else if (c == '\'') {
                put_line(x + 5, y, x + 5, y + 2, color);
            }
            //"
            else if (c == '"') {
                put_line(x + 3, y, x + 3, y + 2, color);
                put_line(x + 7, y, x + 7, y + 2, color);
            }
            //[
            else if (c == '[') {
                //top left to top middle
                put_line(x, y, x + 5, y, color);
                //top left to bottom left
                put_line(x, y, x, y + 20, color);
                //bottom left to bottom middle
                put_line(x, y + 20, x + 5, y + 20, color);
            }
            //]
            else if (c == ']') {
                //top right to top middle
                put_line(x + 5, y, x + 10, y, color);
                //top right to bottom right
                put_line(x + 10, y, x + 10, y + 20, color);
                //bottom right to bottom middle
                put_line(x + 5, y + 20, x + 10, y + 20, color);
            }
            //{
            else if (c == '{') {
                put_line(x, y, x + 5, y, color);
                put_line(x, y, x, y + 20, color);
                put_line(x, y + 20, x + 5, y + 20, color);
                put_line(x + 5, y, x + 5, y + 20, color);
            }
            //}
            else if (c == '}') {
                put_line(x, y, x + 5, y, color);
                put_line(x + 5, y, x + 5, y + 20, color);
                put_line(x, y + 20, x + 5, y + 20, color);
                put_line(x, y, x, y + 20, color);
            }
            //+
            else if (c == '+') {
                //horizontal line
                put_line(x, y + 10, x + 10, y + 10, color);
                // vertical line is the same length as the horizontal line
                put_line(x + 5, y + 5, x + 5, y + 15, color);
            }
            //-
            else if (c == '-') {
                put_line(x, y + 10, x + 10, y + 10, color);
            }
            //_
            else if (c == '_') {
                put_line(x, y + 20, x + 10, y + 20, color);
            }
            //=
            else if (c == '=') {
                put_line(x, y + 5, x + 10, y + 5, color);
                put_line(x, y + 15, x + 10, y + 15, color);
            }
            //*
            else if (c == '*') {
                put_line(x, y, x + 10, y + 20, color);
                put_line(x + 10, y, x, y + 20, color);
                put_line(x, y + 10, x + 10, y + 10, color);
                put_line(x + 5, y, x + 5, y + 20, color);
            }
            // /
            else if (c == '/') {
                put_line(x, y + 20, x + 10, y, color);
            }
            else if (c == '\\') {
                put_line(x, y + 20, x + 10, y, color);
            }
            // <
            else if (c == '<') {
                put_line(x, y + 10, x + 10, y, color);
                put_line(x, y + 10, x + 10, y + 20, color);
            }
            // >
            else if (c == '>') {
                put_line(x, y, x + 10, y + 10, color);
                put_line(x, y + 20, x + 10, y + 10, color);
            }
            // ^
            else if (c == '^') {
                put_line(x, y, x + 5, y + 10, color);
                put_line(x + 5, y + 10, x + 10, y, color);
            }
            // &
            else if (c == '&') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 10, color);
                put_line(x, y + 10, x + 10, y + 20, color);
                put_line(x + 10, y + 20, x + 10, y + 10, color);
            }
            // %
            else if (c == '%') {
                //small rect top left, line from bottom left to top right, small rect bottom right
                put_rect(x, y, 4, 4, color);
                put_rect(x + 6, y + 16, 4, 4, color);
                put_line(x, y + 20, x + 10, y, color);
            }
            // $
            else if (c == '$') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 10, color);
                put_line(x, y + 10, x + 10, y + 10, color);
                put_line(x + 10, y + 10, x + 10, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
                put_line(x, y + 10, x, y + 20, color);
                put_line(x + 10, y, x + 10, y + 10, color);
            }
            // #
            else if (c == '#') {
                //third from top left to third from bottom left
                put_line(x + 3, y, x + 3, y + 20, color);
                //third from top right to third from bottom right
                put_line(x + 7, y, x + 7, y + 20, color);
                // third from top left to middle left to third from top right to middle right
                put_line(x, y + 7, x + 10, y + 7, color);
                // third from bottom left to middle left to third from bottom right to middle right
                put_line(x, y + 13, x + 10, y + 13, color);

            }
            // @
            else if (c == '@') {
                put_line(x, y, x + 10, y, color);
                put_line(x, y, x, y + 20, color);
                put_line(x, y + 20, x + 10, y + 20, color);
                put_line(x + 10, y, x + 10, y + 20, color);
                put_line(x + 5, y + 10, x + 10, y + 10, color);
                put_line(x + 5, y + 10, x + 5, y + 20, color);
            }
            // |
            else if (c == '|') {
                put_line(x + 5, y, x + 5, y + 20, color);
            }
            // ~
            else if (c == '~') {
                put_line(x, y + 10, x + 5, y + 5, color);
                put_line(x + 5, y + 5, x + 10, y + 10, color);
                put_line(x, y + 15, x + 5, y + 10, color);
                put_line(x + 5, y + 10, x + 10, y + 15, color);
            }
            // `
            else if (c == '`') {
                put_line(x + 5, y, x + 7, y + 2, color);
            }
            // else draw box with question mark in it
}
void Graphics::put_line(int x1, int y1, int x2, int y2, int color) {
    int dx = MathI::abs(x2 - x1);
    int dy = MathI::abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;
    while (true) {
        put_pixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x1 += sx; }
        if (e2 < dy) { err += dx; y1 += sy; }
    }
}
void Graphics::put_rect(int x, int y, int w, int h, int color) {
    put_line(x, y, x + w, y, color);
    put_line(x, y, x, y + h, color);
    put_line(x + w, y, x + w, y + h, color);
    put_line(x, y + h, x + w, y + h, color);
}
void Graphics::put_filled_rect(int x, int y, int w, int h, int color) {
    for (int i = 0; i < h; i++) {
        put_line(x, y + i, x + w, y + i, color);
    }
}
void Graphics::put_circle(int x0, int y0, int radius, int color) {
    int x = radius;
    int y = 0;
    int err = 0;
    while (x >= y) {
        put_pixel(x0 + x, y0 + y, color);
        put_pixel(x0 + y, y0 + x, color);
        put_pixel(x0 - y, y0 + x, color);
        put_pixel(x0 - x, y0 + y, color);
        put_pixel(x0 - x, y0 - y, color);
        put_pixel(x0 - y, y0 - x, color);
        put_pixel(x0 + y, y0 - x, color);
        put_pixel(x0 + x, y0 - y, color);
        if (err <= 0) {
            y += 1;
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}
void Graphics::put_filled_circle(int x0, int y0, int radius, int color) {
    int x = radius;
    int y = 0;
    int err = 0;
    while (x >= y) {
        put_line(x0 - x, y0 + y, x0 + x, y0 + y, color);
        put_line(x0 - y, y0 + x, x0 + y, y0 + x, color);
        put_line(x0 - x, y0 - y, x0 + x, y0 - y, color);
        put_line(x0 - y, y0 - x, x0 + y, y0 - x, color);
        if (err <= 0) {
            y += 1;
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}
void Graphics::Swap()
{
    //put all the SwapBuffer data into the framebuffer
    for (int y = 0; y < framebuffer.height; y++) {
        for (int x = 0; x < framebuffer.width; x++) {
            volatile uint32_t *fb_ptr = static_cast<volatile uint32_t *>(framebuffer.address);
            fb_ptr[y * (framebuffer.pitch / 4) + x] = SwapBuffer[y * (framebuffer.pitch / 4) + x];
        }
    }
}

void Graphics::put_image(int x, int y, BMPI Bimage)
{
    int width = Bimage.width;
    int height = Bimage.height;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            put_pixel(x + j, y + i, Bimage.data[i * width + j]);
        }
    }
}

void Graphics::put_pixel_alpha(int x, int y, long color)
{
    if ((color & 0xFF) == 0x00) {
        return;
    }
    //get pixel below the alpha pixel and blend
    volatile uint32_t *fb_ptr = static_cast<volatile uint32_t *>(SwapBuffer);
    int pixel = fb_ptr[y * (framebuffer.pitch / 4) + x];
    int r = (color & 0xFF0000) >> 16;
    int g = (color & 0xFF00) >> 8;
    int b = (color & 0xFF);
    int a = (color & 0xFF000000) >> 24;
    int r2 = (pixel & 0xFF0000) >> 16;
    int g2 = (pixel & 0xFF00) >> 8;
    int b2 = (pixel & 0xFF);
    int a2 = (pixel & 0xFF000000) >> 24;
    int r3 = (r * a + r2 * (255 - a)) / 255;
    int g3 = (g * a + g2 * (255 - a)) / 255;
    int b3 = (b * a + b2 * (255 - a)) / 255;
    int a3 = (a * a + a2 * (255 - a)) / 255;
    fb_ptr[y * (framebuffer.pitch / 4) + x] = (a3 << 24) | (r3 << 16) | (g3 << 8) | b3;
}

void Graphics::put_image_alpha(int x, int y, BMPA Bimage)
{
    int width = Bimage.width;
    int height = Bimage.height;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            put_pixel_alpha(x + j, y + i, Bimage.data[i * width + j]);
        }
    }
}

int Graphics::get_pixel(int x, int y)
{
    volatile uint32_t *fb_ptr = static_cast<volatile uint32_t *>(framebuffer.address);
    return fb_ptr[y * (framebuffer.pitch / 4) + x];
}

void Graphics::put_char_new(char c, int x, int y, int color)
{
    // Assume each character is 16x16 pixels
    const int glyph_width = 16;
    const int glyph_height = 16;
    const int image_width = 256;

    // Pointer to the start of the font data
    long *font = (long*)CourierNew;

    // Calculate the starting position of the character 'c' in the font data
    int char_index = c;
    long *char_data = font + (char_index * glyph_width * glyph_height);

    // Draw the character at position (x, y)
    for (int i = 0; i < glyph_height; i++)
    {
        for (int j = 0; j < glyph_width; j++)
        {
            // Calculate the source pixel position in the character data
            int pixel_index = i * glyph_width + j;

            // Get the pixel value (assuming monochrome for simplicity)
            long pixel_value = char_data[pixel_index];

            // If pixel_value is set, draw it with the specified color
            if (pixel_value)
            {
                // Calculate the target pixel position in the image
                int target_x = x + j;
                int target_y = y + i;

                // Ensure we're within image boundaries
                if (target_x >= 0 && target_x < image_width && target_y >= 0 && target_y < image_width)
                {
                    // Set the pixel at (target_x, target_y) to the specified color
                    // Assuming you have a method set_pixel to set a pixel color in the image
                    put_pixel(target_x, target_y, color);
                }
            }
        }
    }
}


void Graphics::put_string_new(char *str, int x, int y, int color)
{
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        put_char_new(str[i], x + i * 16, y, color);
    }
}
