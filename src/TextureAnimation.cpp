#include "TextureAnimation.h"
#include "Utilities.h"

TextureAnimation::TextureAnimation(Shader* shader, unsigned int uCount, unsigned int vCount)
    : shader(shader), uCount(uCount), vCount(vCount)
{
    Update();
    m_UVOffset[0] = 0.0f;
    m_UVOffset[1] = 0.0f;

    m_UVScale[0] = 1.0f / uCount;
    m_UVScale[1] = 1.0f / vCount;
}

void TextureAnimation::Update()
{

    if (GetTime() - lastUpdateTime >= 0.1f)
    {
        float offset = 1.0f;
        uvOffsetNum++;
        m_UVOffset[0] = uvOffsetNum % uCount;
        m_UVOffset[1] = (uvOffsetNum - m_UVOffset[0]) / uCount;
        uvOffsetNum = uvOffsetNum % 56;
        SetShaderValue(*shader, GetShaderLocation(*shader, "uvOffset"), m_UVOffset, SHADER_UNIFORM_VEC2);
        SetShaderValue(*shader, GetShaderLocation(*shader, "uvScale"), m_UVScale, SHADER_UNIFORM_VEC2);
        lastUpdateTime = GetTime();
    }

}
