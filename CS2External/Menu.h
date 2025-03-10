#pragma once
#include <Windows.h>

class Menu {
public:
    struct Config {
        bool espEnabled = true;
        bool boxEsp = true;
        bool healthEsp = true;
        bool nameEsp = true;
    };

    static bool Init();
    static void Shutdown();
    static Config config;

private:
    static void CreateControls(HWND parent);
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    static HWND hwnd;
    static HWND espBox;
    static HWND boxEspBox;
    static HWND healthEspBox;
    static HWND nameEspBox;
};