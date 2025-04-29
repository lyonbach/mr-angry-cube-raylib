#pragma once
#include "raylib.h"
#include <string>

class Hud
{
public:
    void Render();
    static std::string Vector3ToString(Vector3& vector);
};