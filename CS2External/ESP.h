#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Memory.h"
#include "Menu.h"
#include "Types.h"

class ESP {
public:
    static void RenderDX11(Memory& mem, ID3D11Device* device, ID3D11DeviceContext* context,
        ID3D11Buffer* vertexBuffer, ID3D11Buffer* fontBuffer,
        int width, int height);

private:
    static void DrawBox(ID3D11DeviceContext* context, ID3D11Buffer* vertexBuffer,
        float x, float y, float w, float h, const DirectX::XMFLOAT4& color);

    static void DrawHealthBar(ID3D11DeviceContext* context, ID3D11Buffer* vertexBuffer,
        float x, float y, float h, float health, const DirectX::XMFLOAT4& color);

    static void DrawString(ID3D11DeviceContext* context, ID3D11Buffer* fontBuffer,
        const char* text, float x, float y, const DirectX::XMFLOAT4& color);
};