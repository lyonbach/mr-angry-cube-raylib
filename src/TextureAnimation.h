#pragma once
#include "raylib.h"

class TextureAnimation
{
public:

    TextureAnimation(Shader* shader, unsigned int uCount, unsigned int vCount);
    void Update();

    Shader* shader = nullptr;
    float lastUpdateTime = 0.0f;
    unsigned int uvOffsetNum = 0.0f;
    unsigned int uCount = 0.0f;
    unsigned int vCount = 0.0f;

private:
    float m_UVOffset[2];
    float m_UVScale[2];
};
