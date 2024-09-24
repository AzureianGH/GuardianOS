#include <Taskbar/Taskbar.h>
Graphics* GraphicsObjectTask;
Taskbar::Taskbar(Graphics* GraphicsObject)
{
    currentyear = 0;
    currentmonth = 0;
    currentday = 0;
    currenthour = 0;
    currentminute = 0;
    currentsecond = 0;
    is24hour = false;
    PM = false;
    GraphicsObjectTask = GraphicsObject;
}
Taskbar::~Taskbar()
{
    currentyear = 0;
    currentmonth = 0;
    currentday = 0;
    currenthour = 0;
    currentminute = 0;
    currentsecond = 0;
    is24hour = false;
    PM = false;
}
StringObj FormatNumber(int number, int width) {
    // Convert the number to a string
    StringObj numStr = (StringObj)ToString(number);
    // Add leading zeros if the number is shorter than the desired width
    while (numStr.Length() < width) {
        numStr = (StringObj)"0" + numStr;
    }
    return numStr;
}
int TBCOLOR = 0x323c4d;
int TBBOCOLOR = 0xFFFFFF;
int TIMECOLOR = 0xFFFFFF;
int DATECOLOR = 0xFFFFFF;
int fps = 0;
int frames = 0;
int last = 0;
void Taskbar::ChangeTaskbarColor(int colorbg, int colorborder)
{
    TBCOLOR = colorbg;
    TBBOCOLOR = colorborder;
}
void Taskbar::ChangeTimeColor(int color)
{
    TIMECOLOR = color;
}
void Taskbar::ChangeDateColor(int color)
{
    DATECOLOR = color;
}
void Taskbar::Draw()
{
    currentyear = TimeGetYear();
    currentmonth = TimeGetMonth();
    currentday = TimeGetDay();
    if (is24hour)
    {
        Time_t time = TimeGetTime();
        currenthour = time.Hours;
        currentminute = time.Minutes;
        currentsecond = time.Seconds;
    }
    else
    {
        Time_t time = TimeGetTime12();
        currenthour = time.Hours;
        currentminute = time.Minutes;
        currentsecond = time.Seconds;
        PM = time.PM;
    }
    int mx = GetMouseXPos();
    int my = GetMouseYPos();
    // Draw background and borders
    GraphicsObjectTask->DrawFilledRectangle(0, 0, GraphicsObjectTask->Width - 1, 40, TBCOLOR);
    GraphicsObjectTask->DrawRectangle(0, 0, GraphicsObjectTask->Width - 1, 40, TBBOCOLOR);

    // Set the base position for the text
    int baseX1 = GraphicsObjectTask->Width - 90; // Position from the right edge
    int baseX = GraphicsObjectTask->Width - 115; // Position from the right edge
    int baseY = 3; // Start Y position

    // Draw time string
    StringObj timeString = FormatNumber(currenthour, 2) + ":" + FormatNumber(currentminute, 2) + ":" + FormatNumber(currentsecond, 2) + (is24hour ? "" : (PM ? " PM" : " AM"));
    GraphicsObjectTask->DrawString(timeString, baseX, baseY, TIMECOLOR);

    baseY += 16;
    StringObj dateString = FormatNumber(currentmonth, 2) + "/" + FormatNumber(currentday, 2) + "/" + currentyear;
    GraphicsObjectTask->DrawString(dateString, baseX1, baseY, DATECOLOR);

    
    GraphicsObjectTask->DrawString((StringObj)"System FPS: " + fps, 3, 3, DATECOLOR);
    GraphicsObjectTask->DrawString((StringObj)"Graphics FPS: " + GraphicsObjectTask->GetDrawFPS(), 3, 20, DATECOLOR);

    frames++;
    //use rtc to get time in seconds getSeconds();
    if (TimeGetSeconds() != last)
    {
        last = TimeGetSeconds();
        fps = frames;
        frames = 0;
    }
}
