// Нативное меню на WinAPI

#pragma once
#include <Windows.h>

class Menu {
public:
    struct Config {
        // ESP настройки
        bool espEnabled = true;
        bool boxEsp = true;
        bool healthEsp = true;
        bool nameEsp = true;
        struct {
            int r = 255;
            int g = 0;
            int b = 0;
        } espColor;
    };

    static bool Init();
    static void Shutdown();
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    static inline Config config;
    static inline HWND hwnd = nullptr;
    static inline HWND espBox = nullptr;
    static inline HWND boxEspBox = nullptr;
    static inline HWND healthEspBox = nullptr;
    static inline HWND nameEspBox = nullptr;

private:
    static void CreateControls(HWND parent);
};