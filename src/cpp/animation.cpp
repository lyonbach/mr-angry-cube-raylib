#include "animation.hpp"

Animation::Animation(Shader s, Texture2D t, int f) 
    : shader(s), texture(t), frameCount(f), currentFrame(0) {}

Animation::~Animation() {}

void Animation::Update() {
    int locMove = GetShaderLocation(shader, "uMoveBehaviourIndex");
    int locOffset = GetShaderLocation(shader, "uvOffset");
    int locScale = GetShaderLocation(shader, "uvScale");

    // Update frame based on time (e.g., 10 frames per second)
    currentFrame = (int)(GetTime() * 10.0f) % frameCount;

    // Assuming a 3x3 sprite sheet for 9 frames
    float scaleX = 1.0f / 3.0f;
    float scaleY = 1.0f / 3.0f;
    Vector2 uvScale = { scaleX, scaleY };
    SetShaderValue(shader, locScale, &uvScale, SHADER_UNIFORM_VEC2);

    float offsetX = (float)(currentFrame % 3);
    float offsetY = (float)(currentFrame / 3);
    Vector2 uvOffset = { offsetX, offsetY };
    SetShaderValue(shader, locOffset, &uvOffset, SHADER_UNIFORM_VEC2);

    float currentBehavior = 1.0f;
    SetShaderValue(shader, locMove, &currentBehavior, SHADER_UNIFORM_FLOAT);
}
