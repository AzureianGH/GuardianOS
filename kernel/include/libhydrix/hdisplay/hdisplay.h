#pragma once

class DisplayInfo {
    public:
    void *Address;
    int Bpp; // bits per pixel
    int Width; // width in pixels
    int Height; // height in pixels
    int Refresh; // refresh rate in Hz
    void Init(void *address, int bpp, int width, int height, int refresh) {
        Address = address;
        Bpp = bpp;
        Width = width;
        Height = height;
        Refresh = refresh;
    }
};