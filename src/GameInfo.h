#pragma once
#include <vector>

class GameInfo
{
public:
    int score = 0;
    int faceHits = 0;
    int anger = 0;
    int rotationCountdown = 20;
    Vector3 rotationCount = {0.0f, 0.0f, 0.0f};
    std::vector<float> possibleSpeeds = {
        1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 5.0f, 6.0f, 7.5, 9.0f
    };
};