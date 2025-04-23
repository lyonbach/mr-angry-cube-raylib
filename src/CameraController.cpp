#include "CameraController.h"
#include "Game.h"


CameraController::CameraController()
{
    m_Camera.position = (Vector3){ 0.0f, 0.0f, 0.0f };  // Camera position
    m_Camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    m_Camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    m_Camera.fovy = 70.0f;                                // Camera field-of-view Y
    m_Camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type
};

void CameraController::Update(float deltaTime, GameObject* targetObject)
{
    if (targetObject == nullptr)
    {
        return;
    }
    
    Game& game = Game::Get();
    if (game.mrAngryCube->IsAtQuarterRotation() && (int)game.mrAngryCube->rotationAxis.y == 0)
    {
        game.gameInfo.cameraShakeStrenght = 0.1f;
        game.gameInfo.cameraShakeStrengthLastSet = GetTime();
    } else {
        if (GetTime() - game.gameInfo.cameraShakeStrengthLastSet > 0.15)
        {
            game.gameInfo.cameraShakeStrenght = 0.0f;
        }
    }

    Vector3 target = (Vector3){targetObject->transform.m12, targetObject->transform.m13, targetObject->transform.m14};
    Vector3 &nextRotationAxis = Game::Get().mrAngryCube->nextRotationAxis;
    
    float offsetAmount = 5.0f;
    Vector3 scaledNextRotationAxis = Vector3Scale(nextRotationAxis, offsetAmount);
    std::swap(scaledNextRotationAxis.x, scaledNextRotationAxis.z);
    scaledNextRotationAxis.x *= -1;
    scaledNextRotationAxis.y = 0.0f;

    Vector3 distance = Vector3Add(target, scaledNextRotationAxis) - m_Camera.target;
    Vector3 update = Vector3Scale(distance, deltaTime);
    m_Camera.target = Vector3Add(m_Camera.target, update);

    m_Camera.target.x += GetRandomValue(-1, 1) * Game::Get().gameInfo.cameraShakeStrenght;
    m_Camera.target.y += GetRandomValue(-1, 1) * Game::Get().gameInfo.cameraShakeStrenght;
    m_Camera.target.z += GetRandomValue(-1, 1) * Game::Get().gameInfo.cameraShakeStrenght;
    m_Camera.position = Vector3Add(m_Camera.target, chaseVector);
}

void CameraController::Render()
{
    BeginMode3D(m_Camera);
    DrawGrid(200, 1.0f);
    for (auto& gameObject : Game::Get().gameObjects) { gameObject->Render(); }
    EndMode3D();
}

// void CameraController::DoCameraShake(float duration, float strength, GameObject* targetObject)
// {
//     // FIXME I DONT LIKE THIS SOLUTION, IMPLEMENT APPLYING FORCE APPROACH.
//     std::thread([this, duration, strength, targetObject]() {
//         canUpdate = false;
//         int maxCount = 4;
//         float pause = duration / (maxCount * 2.0f);
//         for(int i = 0; i < maxCount; i++)
//         {
//             Update(targetObject);
//             m_Camera.target.x = targetObject->transform.m12 + 0.1f * strength * (maxCount - i);
//             std::this_thread::sleep_for(std::chrono::duration<float>(pause));
//             Update(targetObject);
//             m_Camera.target.x = targetObject->transform.m12 - 0.1f * strength * (maxCount - i);
//             std::this_thread::sleep_for(std::chrono::duration<float>(pause));
//         }
//         canUpdate = true;
//     }).detach();
// }

Vector3 CameraController::GetFrontVector()
{
    return Vector3Normalize(Vector3Scale(Vector3Multiply(Vector3({1.0f, 0.0f, 1.0f}), chaseVector), -1.0f));
}

Vector3 CameraController::GetRearVector()
{
    return Vector3Normalize(Vector3Multiply(chaseVector, Vector3({1.0f, 0.0f, 1.0f})));
}

Vector3 CameraController::GetLeftVector()
{
    return Vector3Normalize(Vector3CrossProduct(GetFrontVector(), {0.0f, 1.0f, 0.0f}));
}

Vector3 CameraController::GetRightVector()
{
    return Vector3Normalize(Vector3CrossProduct({0.0f, 1.0f, 0.0}, GetFrontVector()));
}

void CameraController::RotateCamera(RotationDirection direction)
{
    float angle = direction == RotationDirection::CCW ? -90.0f : 90.0f;
    chaseVector = Vector3RotateByAxisAngle(chaseVector, {0.0f, 1.0f, 0.0f}, DEG2RAD * angle);
    chaseVector = Utilities::QuantizeVector3(chaseVector);
}