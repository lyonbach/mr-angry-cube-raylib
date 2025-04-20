#pragma once
#include "TimedText.h"
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

namespace Utilities
{
    TimedText* GetTimedText(char const* text);
    TimedText* GetTimedText(char const* text, Vector2 position);
    Vector3 AbsVector3(Vector3& vector3);
    int SumVector3(Vector3& vector3);
    void Log(std::string message, std::string prefix="CUSTOM", TraceLogLevel logLevel=LOG_INFO);
} // namespace Utilities
