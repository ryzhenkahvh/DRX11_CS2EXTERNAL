#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Memory.h"
#include "Types.h"

class Overlay {
public:
    static bool Init();
    static void Shutdown();
    static void Render();
    static Memory* g_memory;

private:
    static bool InitShaders();
    static bool CreateBuffers();
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    static ID3D11Device* device;
    static ID3D11DeviceContext* context;
    static IDXGISwapChain* swapChain;
    static ID3D11RenderTargetView* renderTarget;
    static ID3D11VertexShader* vertexShader;
    static ID3D11PixelShader* pixelShader;
    static ID3D11InputLayout* inputLayout;
    static ID3D11Buffer* vertexBuffer;
    static ID3D11Buffer* fontBuffer;
    static HWND hwnd;
    static int width;
    static int height;
};