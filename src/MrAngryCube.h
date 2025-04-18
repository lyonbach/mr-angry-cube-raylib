#pragma once
#include "Game.h"
#include "GameObject.h"
#include "Miscellaneous.h"
#include "raylib.h"
#include "raymath.h"
#include <vector>


class MrAngryCube : public GameObject
{
public:
    MrAngryCube(const char* texturePath, const char* shaderPath, const char* modelPath);
    virtual ~MrAngryCube() = default;
    void Render() override;
    void Update(float deltaTime) override;
    bool IsFaceOnTheGround();
    bool IsAtQuarterRotation(bool ommitZero=true);
    Vector3 rotation;
    Vector3 nextRotationAxis = { 0.0f, 0.0f, 0.0f };
    Vector3 rotationAxis = { 0.0f, 0.0f, 0.0f };
    Vector3 rotationCount = { 0.0f, 0.0f, 0.0f };
    float speed;
    const void WaitForNonBlocking(float seconds);
    bool isMoving = true;

private:
    float m_Size;
    float m_HalfSize;
    float m_Hypotenuse;
    float m_LastPauseCheckTime = 0.0f;
};
