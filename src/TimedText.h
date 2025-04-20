#pragma once
#include <functional>

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
