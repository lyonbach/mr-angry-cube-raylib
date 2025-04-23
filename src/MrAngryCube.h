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
    Vector3 nextRotationAxis = { 0.0f, 0.0f, 0.0f };
    Vector3 rotationAxis     = { 0.0f, 0.0f, 0.0f };
    Vector3 rotation         = {0.0f, 0.0f, 0.0f};
    float speed              = 0.0f;
    bool isMoving            = false;

    MrAngryCube(const char* texturePath, const char* shaderPath, const char* modelPath);
    virtual ~MrAngryCube() = default;
    void Render() override;
    void Update(float deltaTime) override;
    bool IsFaceOnTheGround();
    bool IsAtQuarterRotation(bool ommitZero=true);
    void WaitFor(float seconds);

private:
    float m_LastMovementCheckTime = 0.0f;
    float m_Size = 0.0f;
    float m_HalfSize = 0.0f;
    float m_Hypotenuse = 0.0f;
};
