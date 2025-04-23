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

enum class Reason
{
    FaceHit,
    Dizzyness,
    Smash,
    AlmostGameOver
};


class TimedText
{
public:
    const std::function<void()> drawFunction;
    float lastCheckTime;
    float duration;
    const char* text;
    Reason reason;

    void Draw();
    TimedText(const std::function<void()> drawFunction, Reason reason);
};


namespace Utilities
{
    TimedText* GetTimedText(char const* text, Reason reason);
    Vector3 AbsVector3(Vector3& vector3);
    Vector3 QuantizeVector3(Vector3& vector3);
    int SumVector3(Vector3& vector3);
    void Log(std::string message, std::string prefix="CUSTOM", TraceLogLevel logLevel=LOG_INFO);
    const char* GetQuote(Reason reason);

}
