#include "Menu.h"
#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")

HWND Menu::hwnd = nullptr;
HWND Menu::espBox = nullptr;
HWND Menu::boxEspBox = nullptr;
HWND Menu::healthEspBox = nullptr;
HWND Menu::nameEspBox = nullptr;
Menu::Config Menu::config;

void Menu::CreateControls(HWND parent) {
    espBox = CreateWindowEx(
        0, L"BUTTON", L"ESP Enabled",
        WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        20, 20, 100, 20,
        parent, nullptr, GetModuleHandle(nullptr), nullptr
    );
    SendMessage(espBox, BM_SETCHECK, config.espEnabled ? BST_CHECKED : BST_UNCHECKED, 0);

    boxEspBox = CreateWindowEx(
        0, L"BUTTON", L"Box ESP",
        WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        20, 50, 100, 20,
        parent, nullptr, GetModuleHandle(nullptr), nullptr
    );
    SendMessage(boxEspBox, BM_SETCHECK, config.boxEsp ? BST_CHECKED : BST_UNCHECKED, 0);

    healthEspBox = CreateWindowEx(
        0, L"BUTTON", L"Health ESP",
        WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        20, 80, 100, 20,
        parent, nullptr, GetModuleHandle(nullptr), nullptr
    );
    SendMessage(healthEspBox, BM_SETCHECK, config.healthEsp ? BST_CHECKED : BST_UNCHECKED, 0);

    nameEspBox = CreateWindowEx(
        0, L"BUTTON", L"Name ESP",
        WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        20, 110, 100, 20,
        parent, nullptr, GetModuleHandle(nullptr), nullptr
    );
    SendMessage(nameEspBox, BM_SETCHECK, config.nameEsp ? BST_CHECKED : BST_UNCHECKED, 0);
}

bool Menu::Init() {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"CS2ExternalMenu";

    if (!RegisterClassEx(&wc))
        return false;

    hwnd = CreateWindowEx(
        0,
        L"CS2ExternalMenu",
        L"CS2 External",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        300, 400,
        nullptr, nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    if (!hwnd)
        return false;

    CreateControls(hwnd);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return true;
}

void Menu::Shutdown() {
    if (hwnd) {
        DestroyWindow(hwnd);
        hwnd = nullptr;
    }
    UnregisterClass(L"CS2ExternalMenu", GetModuleHandle(nullptr));
}

LRESULT CALLBACK Menu::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_COMMAND:
        if ((HWND)lParam == espBox) {
            config.espEnabled = SendMessage(espBox, BM_GETCHECK, 0, 0) == BST_CHECKED;
        }
        else if ((HWND)lParam == boxEspBox) {
            config.boxEsp = SendMessage(boxEspBox, BM_GETCHECK, 0, 0) == BST_CHECKED;
        }
        else if ((HWND)lParam == healthEspBox) {
            config.healthEsp = SendMessage(healthEspBox, BM_GETCHECK, 0, 0) == BST_CHECKED;
        }
        else if ((HWND)lParam == nameEspBox) {
            config.nameEsp = SendMessage(nameEspBox, BM_GETCHECK, 0, 0) == BST_CHECKED;
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}