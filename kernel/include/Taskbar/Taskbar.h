#pragma once
#include <libhydrix/libhydrix.h>
#include <basic.h>

class Taskbar
{
    private:
        static Taskbar* instance;
        int currentyear;
        int currentmonth;
        int currentday;
        int currenthour;
        int currentminute;
        int currentsecond;
        bool is24hour;
        bool PM;
        
    public:
        Taskbar(Graphics* GraphicsObject);
        ~Taskbar();
        void Draw();
        void ChangeTaskbarColor(int colorbg, int colorborder);
        void ChangeTimeColor(int color);
        void ChangeDateColor(int color);
};