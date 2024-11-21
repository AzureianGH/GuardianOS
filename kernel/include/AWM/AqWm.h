#pragma once

// Aqua Window Manager

#include <libhydrix/libhydrix.h>
#include <AWM/Window.h>

class Aqua_Window_Manager
{
    private:
        Vector<Aqua_Window*> Windows;
        Aqua_Window* ActiveWindow;
        void KeyboardWindowManager(KeyboardData_t data);
        void MouseWindowManager(MouseState state);
        static void StaticKeyboardWindowManager(KeyboardData_t data);
        static void StaticMouseWindowManager(MouseState state);
        Graphics* graphics;
    public:
        Aqua_Window_Manager() {}
        Aqua_Window_Manager(Graphics* graphics);
        void AddWindow(Aqua_Window* window);
        Aqua_Window* CreateShallowWindow(uint64_t InitialWidth, uint64_t InitialHeight);
        void RemoveWindow(Aqua_Window* window);
        void SetActiveWindow(Aqua_Window* window);
        Aqua_Window* GetActiveWindow();
        void DrawWindows();
        void SendEvent(Aqua_Window* Window, Aqua_Window_Event Event, void* Data = nullptr);
};
