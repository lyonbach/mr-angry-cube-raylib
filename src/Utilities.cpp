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

void Utilities::LoadFilesFromDirectory(const std::string& directoryPath, const std::string& extension, std::map<std::string, std::string>& mapping)
    {
        for (const auto& entry : fs::directory_iterator(directoryPath))
        {
            if (entry.is_regular_file() && entry.path().extension() == extension)
            {
                std::string fileName = entry.path().stem().string();
                std::string filePath = entry.path().string();
                mapping[fileName] = filePath;
            }
        }
    }

void Utilities::ScheduleEvent(std::function<void()> callback, float time)
{
    ScheduledEvent* timedEvent = new ScheduledEvent(callback, time);
    Game::Get().timedEvents.push_back(timedEvent);
}

void Utilities::ScheduleWarmUp()
{
    Game& game = Game::Get();
    Utilities::Log("Scheduling warmup...", "Utilities", LOG_INFO);  // FIXME Those should be debug.
    Utilities::ScheduleEvent(
        [](){ Utilities::Log("Warmup finished...", "Utilities", LOG_INFO);  // FIXME Those should be debug.
        Game::Get().shouldRender = true;
        }, game.gameConfig->warmUpTime );
}

ScheduledEvent::ScheduledEvent(std::function<void()> callback, float time) : callbackFunction(callback), waitTime(time)
{
    setTime = GetTime();
}

void ScheduledEvent::Update()
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