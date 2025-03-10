#include "ESP.h"
#include "Offsets.h"
#include <string>

using namespace DirectX;

void ESP::DrawBox(ID3D11DeviceContext* context, ID3D11Buffer* vertexBuffer,
    float x, float y, float w, float h, const XMFLOAT4& color) {
    float ndcX = (x / 1920.0f * 2.0f) - 1.0f;
    float ndcY = -(y / 1080.0f * 2.0f) + 1.0f;
    float ndcW = (w / 1920.0f * 2.0f);
    float ndcH = (h / 1080.0f * 2.0f);

    Vertex vertices[] = {
        { XMFLOAT3(ndcX, ndcY, 0.0f), color },
        { XMFLOAT3(ndcX + ndcW, ndcY, 0.0f), color },
        { XMFLOAT3(ndcX + ndcW, ndcY, 0.0f), color },
        { XMFLOAT3(ndcX + ndcW, ndcY - ndcH, 0.0f), color },
        { XMFLOAT3(ndcX + ndcW, ndcY - ndcH, 0.0f), color },
        { XMFLOAT3(ndcX, ndcY - ndcH, 0.0f), color },
        { XMFLOAT3(ndcX, ndcY - ndcH, 0.0f), color },
        { XMFLOAT3(ndcX, ndcY, 0.0f), color }
    };

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, vertices, sizeof(vertices));
    context->Unmap(vertexBuffer, 0);

    context->Draw(8, 0);
}

void ESP::DrawHealthBar(ID3D11DeviceContext* context, ID3D11Buffer* vertexBuffer,
    float x, float y, float h, float health, const XMFLOAT4& color) {
    float barHeight = h * (health / 100.0f);
    XMFLOAT4 healthColor = {
        2.0f * (1.0f - health / 100.0f),
        2.0f * (health / 100.0f),
        0.0f,
        1.0f
    };
    DrawBox(context, vertexBuffer, x, y, 4.0f, barHeight, healthColor);
}

void ESP::DrawString(ID3D11DeviceContext* context, ID3D11Buffer* fontBuffer,
    const char* text, float x, float y, const XMFLOAT4& color) {
    // Реализация рендеринга текста (необходимо реализовать)
}

void ESP::RenderDX11(Memory& mem, ID3D11Device* device, ID3D11DeviceContext* context,
    ID3D11Buffer* vertexBuffer, ID3D11Buffer* fontBuffer, int width, int height) {
    if (!Menu::config.espEnabled) return;

    uintptr_t localPlayer = mem.Read<uintptr_t>(mem.clientDll + Offsets::dwLocalPlayerController);
    if (!localPlayer) return;

    ViewMatrix matrix = mem.Read<ViewMatrix>(mem.clientDll + Offsets::dwViewMatrix);
    uintptr_t entityList = mem.Read<uintptr_t>(mem.clientDll + Offsets::dwEntityList);
    if (!entityList) return;

    for (int i = 1; i <= 64; i++) {
        uintptr_t entity = mem.Read<uintptr_t>(entityList + (i * 0x8));
        if (!entity || entity == localPlayer) continue;

        uintptr_t pawn = mem.Read<uintptr_t>(entity + 0x7BC);
        if (!pawn) continue;

        int health = mem.Read<int>(pawn + 0x32C);
        if (health <= 0 || health > 100) continue;

        Vector3 pos = mem.Read<Vector3>(pawn + 0x1224);
        Vector2 screen;

        if (WorldToScreen(pos, screen, matrix, width, height)) {
            if (Menu::config.boxEsp) {
                DrawBox(context, vertexBuffer,
                    screen.x - 20, screen.y - 50, 40, 60,
                    XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
            }

            if (Menu::config.healthEsp) {
                DrawHealthBar(context, vertexBuffer,
                    screen.x - 25, screen.y - 50, 60,
                    health,
                    XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
            }

            if (Menu::config.nameEsp) {
                char name[32];
                mem.ReadArray(entity + Offsets::Entity::m_szPlayerName, name, sizeof(name));
                DrawString(context, fontBuffer,
                    name,
                    screen.x, screen.y - 65,
                    XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
            }
        }
    }
}