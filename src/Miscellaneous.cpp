#include "Miscellaneous.h"


TimedText::TimedText(const std::function<void()> drawFunction)  
    : drawFunction(drawFunction), lastCheckTime(0.0f), duration(3.0f), text("")
{
    lastCheckTime = GetTime();
};

void TimedText::Draw()
{
    drawFunction();
}

TimedText* Utilities::GetTimedText(char const* text, Vector2 position)  // FIXME create a cpp file instead.
{
    return (new TimedText([text, position](){
            int fontSize = 20;
            DrawText(text, position.x, position.y, fontSize, YELLOW);
        }));
}

TimedText* Utilities::GetTimedText(char const* text)  // FIXME create a cpp file instead.
{
    float x = (GetScreenWidth() - MeasureText(text, 16)) / 2;
    float y = GetScreenHeight() / 3;
    return (new TimedText([text, x, y](){
            int fontSize = 20;
            DrawText(text, x, y, fontSize, YELLOW);
        }));
}

Vector3 Utilities::AbsVector3(Vector3& vector3)
{
    return Vector3({(float)abs(vector3.x), (float)abs(vector3.y), (float)abs(vector3.z)});
}

int Utilities::SumVector3(Vector3& vector3)
{
    return (int)(vector3.x + vector3.y + vector3.z);
}