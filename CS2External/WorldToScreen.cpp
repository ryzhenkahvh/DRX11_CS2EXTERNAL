#include "Types.h"

bool WorldToScreen(const Vector3& pos, Vector2& screen, const ViewMatrix& vm, int width, int height) {
    float w = vm.matrix[12] * pos.x + vm.matrix[13] * pos.y + vm.matrix[14] * pos.z + vm.matrix[15];

    if (w < 0.01f)
        return false;

    float x = vm.matrix[0] * pos.x + vm.matrix[1] * pos.y + vm.matrix[2] * pos.z + vm.matrix[3];
    float y = vm.matrix[4] * pos.x + vm.matrix[5] * pos.y + vm.matrix[6] * pos.z + vm.matrix[7];

    screen.x = (width / 2.0f) * (1.0f + x / w);
    screen.y = (height / 2.0f) * (1.0f - y / w);

    return true;
}