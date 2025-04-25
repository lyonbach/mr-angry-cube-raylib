#pragma once
#include <vector>

class GameInfo
{
public:
    unsigned int const defaultGameOverCountDown = 20;
    unsigned int const defaultAngerIncrementCountdown = 10;
    int score = 0;
    int faceHits = 0;
    int anger = 0;
    int gameOverCountdown = defaultGameOverCountDown;
    int angerIncrementCountdown = defaultAngerIncrementCountdown;
    int lastRotationCount = 0;
    float lastUpdateTime = 0.0f;
    Vector3 rotationCount = {0.0f, 0.0f, 0.0f};
    std::vector<float> possibleSpeeds = {
        1.5f, 2.0f, 2.5f, 3.0f, 4.5f, 5.0f, 6.0f, 7.5, 9.0f
    };
    int maxAnger = possibleSpeeds.size() - 1;
    float cameraShakeStrenght = 0.0f;
    float cameraShakeStrengthLastSet = 0.0f;
};