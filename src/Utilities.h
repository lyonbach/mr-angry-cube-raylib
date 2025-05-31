#pragma once
#include "raylib.h"
#include <string>
#include <map>
#include <functional>

class ScheduledEvent
{
public:
    ScheduledEvent(std::function<void()> callback, float time);
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
    void LoadFilesFromDirectory(const std::string& directoryPath, const std::string& extension, std::map<std::string, std::string>& mapping);
    void ScheduleEvent(std::function<void()> callback, float time);
    void ScheduleWarmUp();
}

