#include "GameConfig.h"
#include "Utilities.h"
#include "rini.h"
#include <fstream>
#include <sstream>


GameConfig::GameConfig()
{
    Utilities::Log("No config path was provided, falling back to defaults.", "GameConfig", LOG_WARNING);
    LogConfig();
}

GameConfig::GameConfig(const char* configPath) : configPath(configPath)
{
    Utilities::Log("Initializing GameConfig with config path: " + std::string(configPath), "GameConfig", LOG_INFO);
    Init();
    LogConfig();
}

void GameConfig::LogConfig()
{
    updateTime = 1.0f / updateRate;
    Utilities::Log("GameConfig State:", "GameConfig", LOG_DEBUG);
    Utilities::Log("  Configured: " + std::string(configured ? "true" : "false"), "GameConfig", LOG_DEBUG);
    Utilities::Log("  FullScreen: " + std::string(fullScreen ? "true" : "false"), "GameConfig", LOG_DEBUG);
    Utilities::Log("  ScreenSize: (" + std::to_string(screenSize.x) + ", " + std::to_string(screenSize.y) + ")", "GameConfig", LOG_DEBUG);
    Utilities::Log("  WarmUpTime: " + std::to_string(warmUpTime) + " (seconds)", "GameConfig", LOG_DEBUG);
    Utilities::Log("  UpdateRate: " + std::to_string(updateRate) + " (times/seconds)", "GameConfig", LOG_DEBUG);
    Utilities::Log("  UpdateTime: " + std::to_string(updateTime) + " (seconds)", "GameConfig", LOG_DEBUG);
}

void GameConfig::LogFilePaths(std::map<std::string, std::string>& paths, std::string typeName) const
{
    for (const auto& pair : paths)
    {
        Utilities::Log(typeName + ": " + pair.second, "GameConfig", LOG_DEBUG);
    }

}

void GameConfig::Init()
{
    auto config = rini_load_config(configPath);
    fullScreen = rini_get_config_value(config, "FULLSCREEN");
    screenSize.x = rini_get_config_value(config, "WIDTH");
    screenSize.y = rini_get_config_value(config, "HEIGHT");
    updateRate = rini_get_config_value(config, "UPDATE_RATE");

    int warmUpTime_ = rini_get_config_value(config, "WARMUP_TIME");
    if (warmUpTime_) {
        warmUpTime = ((float)warmUpTime_) / 1000.0f;
    }

    for (size_t i = 0; i < config.count; ++i)
    {
        auto& [key, text, desc] = config.values[i];
        Utilities::Log("Key: " + std::string(key) + ", Value: " + std::string(text) + ", Description: " + std::string(desc), "GameConfig", LOG_DEBUG);
    }

    updateTime = 1.0f / updateRate;

    fs::path assetsDirectory = fs::path("./") / ASSETS_DIRECTORY;
    Utilities::Log("Gathering texture paths...", "GameConfig", LOG_INFO);  // FIXME DEBUG
    fs::path texturesDirectory = assetsDirectory / TEXTURES_DIRECTORY;
    Utilities::LoadFilesFromDirectory(texturesDirectory.string(), TEXTURE_EXTENSION, texturePaths);
    LogFilePaths(texturePaths, "Texture");
    
    Utilities::Log("Gathering shader paths...", "GameConfig", LOG_INFO);  // FIXME DEBUG
    fs::path shadersDirectory = assetsDirectory / SHADERS_DIRECTORY;
    Utilities::LoadFilesFromDirectory(shadersDirectory.string(), VERTEX_SHADER_EXTENSION, vertexShaderPaths);
    Utilities::LoadFilesFromDirectory(shadersDirectory.string(), FRAGMENT_SHADER_EXTENSION, fragmentShaderPaths);
    shaderPaths["mr-angry-cube-body"] = vertexShaderPaths["base"] + "|" + fragmentShaderPaths["base"];
    shaderPaths["mr-angry-cube-face"] = vertexShaderPaths["base"] + "|" + fragmentShaderPaths["base"];
    shaderPaths["static-object"] = vertexShaderPaths["base"] + "|" + fragmentShaderPaths["base"];
    LogFilePaths(vertexShaderPaths, "Vertex Shader");
    LogFilePaths(fragmentShaderPaths, "Fragment Shader");

    Utilities::Log("Gathering model paths...", "GameConfig");
    fs::path modelsDirectory = assetsDirectory / MODELS_DIRECTORY;
    Utilities::LoadFilesFromDirectory(modelsDirectory.string(), MODEL_EXTENSION, modelPaths);
    LogFilePaths(modelPaths, "Model");

    configured = true;
}