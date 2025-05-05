#include "Utilities.h"


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
