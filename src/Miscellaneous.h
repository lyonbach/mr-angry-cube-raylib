#pragma once
#include "raylib.h"
#include <functional>


enum class GameState {
    MainMenu,
    Playing,
    Paused,
    GameOver
};

struct GameConfig {
    GameConfig(
        int targetFPS,
        int updateSpeed,
        int screenWidth,
        int screenHeight,
        const char* windowTitle,
        const char* texturePath,
        const char* shaderPath,
        const char* modelPath)
        : targetFPS(targetFPS),
        updateSpeed(updateSpeed),
        screenWidth(screenWidth),
        screenHeight(screenHeight),
        texturePath(texturePath),
        shaderPath(shaderPath),
        modelPath(modelPath),
        windowTitle(windowTitle) {}
    int targetFPS;
    int updateSpeed;
    int screenWidth;
    int screenHeight;
    const char* texturePath;  // FIXME use std::string
    const char* shaderPath;  // FIXME use std::string
    const char* modelPath;  // FIXME use std::string
    const char* windowTitle;  // FIXME use std::string
};

struct GameInfo {
    int score = 0;
    int faceHits = 0;
    float anger = 1.0f;
    float maxAnger = 5.0f;
};

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

namespace Utilities
{
    TimedText* GetTimedText(char const* text);
    TimedText* GetTimedText(char const* text, Vector2 position);
} // namespace Utilities


