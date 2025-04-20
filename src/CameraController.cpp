#include "CameraController.h"
#include "Game.h"
#include <thread>


CameraController::CameraController()
{
    m_Camera.position = (Vector3){ 0.0f, 0.0f, 0.0f };  // Camera position
    m_Camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    m_Camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    m_Camera.fovy = 80.0f;                                // Camera field-of-view Y
    m_Camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type
};

void CameraController::Update(GameObject* targetObject)
{
    if (targetObject == nullptr)
    {
        return;
    }

    // FIXME FIND THE FORCE -> SHOULD BE PROPORTIONAL TO THE SQUARE OF THE DISTANCE
    m_Camera.target = (Vector3){targetObject->transform.m12, targetObject->transform.m13, targetObject->transform.m14};
    m_Camera.position = Vector3Add(m_Camera.target, chaseVector);

}

void CameraController::Run(GameObject* targetObject)
{
    std::thread([this, targetObject](){
        MrAngryCube* mrAngryCube = dynamic_cast<MrAngryCube*>(targetObject);
        Vector3 lastRotationCount = Game::Get().gameInfo.rotationCount;

        while (shouldRun && !WindowShouldClose())
        {
            if (canUpdate) { Update(targetObject); }
            // m_Camera.position = (Vector3){m_Camera.target.x, m_Camera.target.y + 2.0f, m_Camera.target.z - 5.0f};
            if (mrAngryCube)
            {
                float shakeStrength = mrAngryCube->IsFaceOnTheGround() ? 1.0f : .2f;
                if (mrAngryCube->IsAtQuarterRotation() && mrAngryCube->isMoving && (lastRotationCount.x != mrAngryCube->rotation.x || lastRotationCount.z != mrAngryCube->rotation.z) > .1f)
                {
                    DoCameraShake(2.0f, shakeStrength, targetObject);
                }
            }
            lastRotationCount = mrAngryCube->rotation;
        }

    }).detach();
}

void CameraController::Render()
{
    BeginMode3D(m_Camera);
    DrawGrid(200, 1.0f);
    for (auto& gameObject : Game::Get().gameObjects) { gameObject->Render(); }
    EndMode3D();
}

void CameraController::DoCameraShake(float seconds, float strength, GameObject* targetObject)
{
    // FIXME I DONT LIKE THIS SOLUTION, IMPLEMENT APPLYING FORCE APPROACH.
    std::thread([this, seconds, strength, targetObject]() {
        canUpdate = false;
        int maxCount = 4;
        for(int i = 0; i < maxCount; i++)
        {
            Update(targetObject);
            m_Camera.target.x = targetObject->transform.m12 + 0.1f * strength * (maxCount - i);
            std::this_thread::sleep_for(std::chrono::duration<float>(.04));
            Update(targetObject);
            m_Camera.target.x = targetObject->transform.m12 - 0.1f * strength * (maxCount - i);
            std::this_thread::sleep_for(std::chrono::duration<float>(.04));
        }
        canUpdate = true;
    }).detach();
}

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
    return Vector3CrossProduct(GetFrontVector(), {0.0f, 1.0f, 0.0f});
}

Vector3 CameraController::GetRightVector()
{
    return Vector3CrossProduct({0.0f, 1.0f, 0.0}, GetFrontVector());
}

void CameraController::RotateCamera(RotationDirection direction)
{
    float angle = direction == RotationDirection::CCW ? -90.0f : 90.0f;
    chaseVector = Vector3RotateByAxisAngle(chaseVector, {0.0f, 1.0f, 0.0f}, DEG2RAD * angle);
}