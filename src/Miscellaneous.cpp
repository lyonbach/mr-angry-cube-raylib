#include "Miscellaneous.h"
#include "raymath.h"


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
    TimedText* timedText = new TimedText([text, x, y](){
        int fontSize = 20;
        DrawText(text, x, y, fontSize, YELLOW);
    });
    timedText->lastCheckTime = GetTime();
    return timedText;
}

Vector3 Utilities::AbsVector3(Vector3& vector3)
{
    return Vector3({(float)abs(vector3.x), (float)abs(vector3.y), (float)abs(vector3.z)});
}

Vector3 Utilities::QuantizeVector3(Vector3& vector3)
{
    Vector3 copyVector3({vector3.x, vector3.y, vector3.z});
    copyVector3.x = round(copyVector3.x * 2.0f) / 2.0f;
    copyVector3.y = round(copyVector3.y * 2.0f) / 2.0f;
    copyVector3.z = round(copyVector3.z * 2.0f) / 2.0f;
    return copyVector3;
}

int Utilities::SumVector3(Vector3& vector3)
{
    return (int)(vector3.x + vector3.y + vector3.z);
}

void Utilities::Log(std::string message, std::string prefix, TraceLogLevel logLevel)
{
    TraceLog(logLevel, ("[" + prefix + "]: " + message).c_str());
}
