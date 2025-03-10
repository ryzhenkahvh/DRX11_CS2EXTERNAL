#pragma once
#include "../Core/Memory.h"
#include "../SDK/Vector.h"
#include "../GUI/Menu.h"
#include <Windows.h>

class ESP {
public:
    static bool WorldToScreen(const Vector3& pos, Vector2& screen, const ViewMatrix& matrix);
    static void Render(Memory& mem, HDC hdc, int screenWidth, int screenHeight);

private:
    static void DrawBox(HDC hdc, const Vector2& top, const Vector2& bottom, const COLORREF& color);
    static void DrawHealthBar(HDC hdc, const Vector2& top, const Vector2& bottom, int health);
    static void DrawString(HDC hdc, const Vector2& pos, const char* text, const COLORREF& color);
};