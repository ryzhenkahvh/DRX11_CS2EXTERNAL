// Для работы с векторами

#pragma once
#include <cmath>

struct Vector2 {
    float x, y;
    Vector2() : x(0.f), y(0.f) {}
    Vector2(float x, float y) : x(x), y(y) {}
};

struct Vector3 {
    float x, y, z;
    Vector3() : x(0.f), y(0.f), z(0.f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    float Distance(const Vector3& other) const {
        return sqrt(
            (x - other.x) * (x - other.x) +
            (y - other.y) * (y - other.y) +
            (z - other.z) * (z - other.z)
        );
    }
};

struct ViewMatrix {
    float matrix[16];
};