#include "Utilities.h"
#include "Game.h"


void Utilities::Log(std::string message, std::string prefix, TraceLogLevel logLevel)
{
    TraceLog(logLevel, ("[" + prefix + "]: " + message).c_str());
}

void Utilities::Log(Vector3 vector, std::string prefix, TraceLogLevel logLevel)
{
    std::string message = "\nX: " + std::to_string(vector.x) + "\nY: " + std::to_string(vector.y) + "\nZ: " + std::to_string(vector.z);
    Log(message, prefix, logLevel);
}

void Utilities::Log(Vector2 vector, std::string prefix, TraceLogLevel logLevel)
{
    std::string message = "\nX: " + std::to_string(vector.x) + "\nY: " + std::to_string(vector.y);
    Log(message, prefix, logLevel);
}

std::string Utilities::GenerateHash()
{
    return std::to_string(GetTime());  // Use current time as object id.
}

void Utilities::ScheduleEvent(std::function<void()> callback, float time)
{
    TimedEvent* timedEvent = new TimedEvent(callback, time);
    Game::Get().timedEvents.push_back(timedEvent);
}

TimedEvent::TimedEvent(std::function<void()> callback, float time) : callbackFunction(callback), waitTime(time)
{
    setTime = GetTime();
}

void TimedEvent::Update()
{
    if (triggered) { return; }
    if (GetTime() - setTime >= waitTime)
    {
        triggered = true;
        if (callbackFunction != nullptr)
        {
            callbackFunction();
        }
    }
}