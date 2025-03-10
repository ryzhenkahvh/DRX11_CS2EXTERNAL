#include <Windows.h>
#include "Memory.h"
#include "ESP.h"
#include "Menu.h"
#include "Overlay.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#pragma comment(linker, "/ENTRY:WinMainCRTStartup")

bool IsGameRunning() {
    HWND gameWindow = FindWindowW(L"SDL_app", L"Counter-Strike 2");
    return (gameWindow != nullptr);
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    // Ждем запуска игры
    while (!IsGameRunning()) {
        Sleep(1000);
    }
    Sleep(2000); // Даем игре время полностью загрузиться

    // Инициализация Memory
    Overlay::g_memory = new Memory(L"cs2.exe");
    int retryCount = 0;
    while (!Overlay::g_memory->processHandle && retryCount < 5) {
        delete Overlay::g_memory;
        Sleep(1000);
        Overlay::g_memory = new Memory(L"cs2.exe");
        retryCount++;
    }

    if (!Overlay::g_memory->processHandle) {
        MessageBoxW(nullptr, L"Failed to open process!", L"Error", MB_OK);
        return 1;
    }

    // Инициализация оверлея и меню
    if (!Overlay::Init()) {
        MessageBoxW(nullptr, L"Failed to initialize overlay!", L"Error", MB_OK);
        delete Overlay::g_memory;
        return 1;
    }

    if (!Menu::Init()) {
        MessageBoxW(nullptr, L"Failed to initialize menu!", L"Error", MB_OK);
        Overlay::Shutdown();
        delete Overlay::g_memory;
        return 1;
    }

    // Основной цикл
    MSG msg;
    ULONGLONG lastFrame = GetTickCount64();

    while (true) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        ULONGLONG currentTime = GetTickCount64();

        // Ограничиваем FPS до ~60
        if (currentTime - lastFrame >= 16) {  // ~60 FPS
            lastFrame = currentTime;
            Overlay::Render();
        }
        else {
            Sleep(1);  // Освобождаем CPU
        }
    }

cleanup:
    // Очистка
    Overlay::Shutdown();
    Menu::Shutdown();
    delete Overlay::g_memory;

    return 0;
}