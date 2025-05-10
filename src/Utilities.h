#pragma once
#include "raylib.h"
#include <string>
#include <functional>

class TimedEvent
{
public:
    TimedEvent(std::function<void()> callback, float time);
    void Update();
    std::function<void()> callbackFunction;
    float setTime;
    float waitTime;
    bool triggered = false;
};

namespace Utilities
{
    void Log(std::string message, std::string prefix="CUSTOM", TraceLogLevel logLevel=LOG_INFO);
    void Log(Vector3 vector, std::string prefix="CUSTOM", TraceLogLevel logLevel=LOG_INFO);
    void Log(Vector2 vector, std::string prefix="CUSTOM", TraceLogLevel logLevel=LOG_INFO);
    std::string GenerateHash();
    std::string GetFilePath(std::string levelName);
    void ScheduleEvent(std::function<void()> callback, float time);
}

