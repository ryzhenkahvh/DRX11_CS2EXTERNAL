#pragma once
#include <DirectXMath.h>

struct Vector2 {
    float x, y;
    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}
};

struct Vector3 {
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct ViewMatrix {
    float matrix[16];
};

struct Vertex {
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT4 color;
};

bool WorldToScreen(const Vector3& pos, Vector2& screen, const ViewMatrix& vm, int width, int height);