#include <AWM/AqWm.h>
uint64_t WinID = 0;
bool dragging = false;
static Aqua_Window_Manager* instance;
void Aqua_Window_Manager::KeyboardWindowManager(KeyboardData_t data)
{
    if (ActiveWindow != nullptr)
    {
        //check if its key up or down
        if (data.pressed == false)
        {
            SendEvent(ActiveWindow, Aqua_Window_Event::KeyRelease);
        }
        else
        {
            SendEvent(ActiveWindow, Aqua_Window_Event::KeyPress);
        }
    }
}
ULPoint Offsetpnt = {0, 0};
void Aqua_Window_Manager::MouseWindowManager(MouseState state)
{
    if (ActiveWindow != nullptr)
    {
        // Check if mouse is over the active window
        if (state.X > ActiveWindow->X && state.X < ActiveWindow->X + ActiveWindow->Width &&
            state.Y > ActiveWindow->Y && state.Y < ActiveWindow->Y + ActiveWindow->Height)
        {
            // Mouse is over the active window
            SendEvent(ActiveWindow, Aqua_Window_Event::MouseMove);

            // Check for mouse press (left button down)
            if (state.State == MOUSE_LEFT && !dragging)
            {
                // Start dragging the window, calculate offset
                dragging = true;
                Offsetpnt.X = state.X - ActiveWindow->X;
                Offsetpnt.Y = state.Y - ActiveWindow->Y;
            }

            // If dragging is active, update the window's position
            if (dragging)
            {
                // Update window position based on mouse movement
                ActiveWindow->X = state.X - Offsetpnt.X;
                ActiveWindow->Y = state.Y - Offsetpnt.Y;
            }

            // Check for mouse release (left button up)
            if (state.State == MOUSE_NONE && dragging)
            {
                // Stop dragging
                dragging = false;
            }
        }
        else
        {
            // Mouse is not over the active window, check for focus change
            for (int i = 0; i < Windows.Length(); i++)
            {
                Aqua_Window* window = Windows[i];
                if (state.X > window->X && state.X < window->X + window->Width &&
                    state.Y > window->Y && state.Y < window->Y + window->Height)
                {
                    SetActiveWindow(window);
                    SendEvent(window, Aqua_Window_Event::Focus);
                    break;
                }
            }
        }
    }
}

void Aqua_Window_Manager::StaticKeyboardWindowManager(KeyboardData_t data)
{
    instance->KeyboardWindowManager(data);
}

void Aqua_Window_Manager::StaticMouseWindowManager(MouseState state)
{
    instance->MouseWindowManager(state);
}


Aqua_Window_Manager::Aqua_Window_Manager(Graphics* gp)
{
    instance = this;
    ActiveWindow = nullptr;
    AddKeyboardInterrupt(StaticKeyboardWindowManager);
    AddMouseInterrupt(StaticMouseWindowManager);
    graphics = gp;
}

void Aqua_Window_Manager::AddWindow(Aqua_Window* window)
{
    Windows.PushBack(window);
    SetActiveWindow(window);
}

Aqua_Window* Aqua_Window_Manager::CreateShallowWindow(uint64_t InitialWidth, uint64_t InitialHeight)
{
    Aqua_Window* window = (Aqua_Window*)KernelAllocate(sizeof(Aqua_Window));
    window->X = 0;
    window->Y = 0;
    window->Width = InitialWidth;
    window->Height = InitialHeight;
    window->Buffer = (uint*)KernelAllocate(InitialWidth * InitialHeight * 4);
    window->WinID = WinID;
    WinID++;
    Windows.PushBack(window);
    return Windows[Windows.Length() - 1];
}

void Aqua_Window_Manager::RemoveWindow(Aqua_Window* window)
{
    Windows.EraseValue(window);
    //get value on top
    if (Windows.Length() > 0)
    {
        ActiveWindow = Windows[Windows.Length() - 1];
    }
    else
    {
        ActiveWindow = nullptr;
    }
}

void Aqua_Window_Manager::SetActiveWindow(Aqua_Window* window)
{
    if (window != nullptr)
    {
        ActiveWindow = window;
    }
}

Aqua_Window* Aqua_Window_Manager::GetActiveWindow()
{
    return ActiveWindow;
}

void Aqua_Window_Manager::DrawWindows()
{
    for (int i = 0; i < Windows.Length(); i++)
    {
        Aqua_Window* window = Windows[i];
        //draw window
        if (window->DrawHandler != nullptr)
        {
            window->DrawHandler(window->Buffer);
        }
    }

    //draw titlebar and border
    for (int i = 0; i < Windows.Length(); i++)
    {
        Aqua_Window* window = Windows[i];
        if (window->HasTitleBar)
        {
            graphics->DrawRectangle(window->X - 1, window->Y + 1, window->Width + 1, window->Height + 33, 0x111111);
            graphics->DrawFilledRectangle(window->X, window->Y, window->Width, 32, 0xFFFFFF);
            graphics->DrawRectangle(window->X, window->Y, window->Width, 32, 0x000000);
            //string is 16 pixels high, put in middle
            graphics->DrawString(window->Title, window->X + 5, window->Y + 8, 0x000000);
            //draw the buffer
            graphics->DrawBitmap(window->Buffer, window->X, window->Y + 32, window->Width, window->Height);
        }
    }
}

void Aqua_Window_Manager::SendEvent(Aqua_Window* Window, Aqua_Window_Event Event, void* Data)
{
    //call the event handler
    Window->EventHandler(Event, Data);
}
