#pragma once
#include "Constants.h"
#include "raylib.h"
#include <vector>

class CameraController
{
public:
    CameraController();
    ~CameraController();
    void Update(float deltaTime);
    void ZoomIn();
    void ZoomOut();
    void MoveUp();
    void MoveDown();
    void LeftView();
    void RightView();
    Vector3 GetFrontVector();
    Vector3 GetBackVector();
    Vector3 GetLeftVector();
    Vector3 GetRightVector();

    Camera* camera;
    float cameraShakeStrenght;

    std::vector<float> possibleCameraVerticalValues = {
        CAMERA_VERTICAL_CHASE,
        CAMERA_VERTICAL_NORMAL,
        CAMERA_VERTICAL_TOP_DOWN
    };
    int cameraVerticalValueIndex = 1;

    Vector3 chaseVector{ 0.0f, 4.0f, 5.0f };
    Vector3 nextPositon{ 0.0f, 4.0f, 5.0f };
};