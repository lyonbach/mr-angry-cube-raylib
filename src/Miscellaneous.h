#pragma once
#include "raylib.h"
#include <functional>
#include <vector>
#include <string>


enum class GameState {
    MainMenu,
    Playing,
    Paused,
    GameOver
};

enum class RotationDirection
{
    CCW,
    CW
};

struct GameInfo {
    int score = 0;
    int faceHits = 0;
    std::vector<float> possibleSpeeds = {
        1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 5.0f, 6.0f, 7.5, 9.0f
    };
    int anger = 0;
    int rotationCountdown = 20;
};

class TimedText
{
public:
    const std::function<void()> drawFunction;
    float lastCheckTime;
    float duration;
    const char* text;

    void Draw();
    TimedText(const std::function<void()> drawFunction);
};

namespace Utilities
{
    TimedText* GetTimedText(char const* text);
    TimedText* GetTimedText(char const* text, Vector2 position);
    Vector3 AbsVector3(Vector3& vector3);
    int SumVector3(Vector3& vector3);
    void Log(std::string message, std::string prefix="CUSTOM", TraceLogLevel logLevel=LOG_INFO);
} // namespace Utilities
