#pragma once
#include "GameObject.h"
#include "Miscellaneous.h"
#include "raylib.h"



class CameraController
{
public:
    bool shouldRun = true;
    bool canUpdate = true;
    Vector3 chaseVector {0.0f, 2.0f, -5.0f}; // FIXME MOVE TO GAMECONFIG

    CameraController();
    void Update(GameObject* targetObject=nullptr);
    void Run(GameObject* targetObject=nullptr);
    void Render();
    Vector3 GetLeftVector();
    Vector3 GetFrontVector();
    Vector3 GetRearVector();
    Vector3 GetRightVector();
    void RotateCamera(RotationDirection direction);
    
private:
    Camera3D m_Camera;
    void DoCameraShake(float duration, float strength, GameObject* targetObject);
};