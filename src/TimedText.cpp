#include "TimedText.h"
#include "raylib.h"


TimedText::TimedText(const std::function<void()> drawFunction)  
    : drawFunction(drawFunction), lastCheckTime(0.0f), duration(3.0f), text("")
{
    lastCheckTime = GetTime();
};

void TimedText::Draw()
{
    drawFunction();
}