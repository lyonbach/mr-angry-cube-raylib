#pragma once

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
    const char* texturePath;
    const char* shaderPath;
    const char* modelPath;
    const char* windowTitle;
};

struct GameInfo {
    int score = 0;
    float anger = 1.0f;
    float maxAnger = 10.0f;
};