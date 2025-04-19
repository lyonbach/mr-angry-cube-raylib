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

    // FIND THE FORCE -> SHOULD BE PROPORTIONAL TO THE SQUARE OF THE DISTANCE
    // SHOULD CAMERA UPDATE BE COMPLETELY ON ANOTHER THREAD?
    //
    m_Camera.target = (Vector3){targetObject->transform.m12, targetObject->transform.m13, targetObject->transform.m14};
    m_Camera.position = (Vector3){m_Camera.target.x, m_Camera.target.y + 2.0f, m_Camera.target.z - 5.0f};

    // Camera Effects.
    // MrAngryCube* mrAngryCube = dynamic_cast<MrAngryCube*>(targetObject);
    // if (mrAngryCube != nullptr)
    // {
    //     if(mrAngryCube->IsFaceOnTheGround())
    //     {
    //         DoCameraShake(2.0f, 2.0f, mrAngryCube);
    //     }
    // }
}

void CameraController::Run(GameObject* targetObject)
{
    std::thread([this, targetObject](){
        MrAngryCube* mrAngryCube = dynamic_cast<MrAngryCube*>(targetObject);
        Vector3 lastRotation = mrAngryCube->rotationCount;

        while (shouldRun && !WindowShouldClose())
        {
            if (canUpdate)
            {
                Vector3 distanceVector = Vector3Subtract(targetObject->GetPosition(), m_Camera.target);  // FIXME
                m_Camera.target = Vector3Scale(distanceVector, .1) + m_Camera.target; // FIXME 
            }
            m_Camera.position = (Vector3){m_Camera.target.x, m_Camera.target.y + 2.0f, m_Camera.target.z - 5.0f};
            if (mrAngryCube)
            {
                float shakeStrength = mrAngryCube->IsFaceOnTheGround() ? 1.0f : .2f; 
                if (mrAngryCube->IsAtQuarterRotation() && mrAngryCube->isMoving && (lastRotation.x != mrAngryCube->rotation.x || lastRotation.z != mrAngryCube->rotation.z) > .1f)
                {
                    DoCameraShake(2.0f, shakeStrength, targetObject);
                }
            }
            lastRotation = mrAngryCube->rotation;
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
    std::thread([this, seconds, strength, targetObject]() {
        canUpdate = false;
        int maxCount = 4;
        for(int i = 0; i < maxCount; i++)
        {
            m_Camera.target = (Vector3){targetObject->transform.m12, targetObject->transform.m13, targetObject->transform.m14};
            m_Camera.target.x = targetObject->transform.m12 + 0.1f * strength * (maxCount - i);
            std::this_thread::sleep_for(std::chrono::duration<float>(.04));
            m_Camera.target = (Vector3){targetObject->transform.m12, targetObject->transform.m13, targetObject->transform.m14};
            m_Camera.target.x = targetObject->transform.m12 - 0.1f * strength * (maxCount - i);
            std::this_thread::sleep_for(std::chrono::duration<float>(.04));
        }
        canUpdate = true;
    }).detach();
}
