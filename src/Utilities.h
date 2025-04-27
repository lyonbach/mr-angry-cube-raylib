#pragma once
#include "raylib.h"
#include <string>


namespace Utilities
{
    void Log(std::string message, std::string prefix="CUSTOM", TraceLogLevel logLevel=LOG_INFO);
    void Log(Vector3 vector, std::string prefix="CUSTOM", TraceLogLevel logLevel=LOG_INFO);
    std::string GenerateHash();
}

