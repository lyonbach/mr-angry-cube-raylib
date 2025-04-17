#pragma once
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
    float speed = 1.0f;  // Degrees per update.
    std::vector<TimedText*> timedTexts;

private:
    const void WaitFor(float seconds);
    float m_Size;
    float m_HalfSize;
    float m_Hypotenuse;
    float m_AngerLevel = 1.0f;  // Mr. Angry Cube is least a little angry.
    bool m_IsMoving = true;
    float m_LastPauseCheckTime = 0.0f;
    float m_MovePauseDuration = 0.2f;
};

