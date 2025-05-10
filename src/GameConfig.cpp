#include "GameConfig.h"
#include "Utilities.h"
#include "rini.h"
#include <fstream>
#include <sstream>


GameConfig::GameConfig()
{
    Utilities::Log("No config path was provided, falling back to defaults.", "GameConfig", LOG_INFO);
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
    Utilities::Log("GameConfig State:", "GameConfig", LOG_INFO);
    Utilities::Log("  Configured: " + std::string(configured ? "true" : "false"), "GameConfig", LOG_INFO);
    Utilities::Log("  FullScreen: " + std::string(fullScreen ? "true" : "false"), "GameConfig", LOG_INFO);
    Utilities::Log("  ScreenSize: (" + std::to_string(screenSize.x) + ", " + std::to_string(screenSize.y) + ")", "GameConfig", LOG_INFO);
    Utilities::Log("  WarmUpTime: " + std::to_string(warmUpTime) + " (seconds)", "GameConfig", LOG_INFO);
    Utilities::Log("  UpdateRate: " + std::to_string(updateRate) + " (times/seconds)", "GameConfig", LOG_INFO);
    Utilities::Log("  UpdateTime: " + std::to_string(updateTime) + " (seconds)", "GameConfig", LOG_INFO);
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
        Utilities::Log("Key: " + std::string(key) + ", Value: " + std::string(text) + ", Description: " + std::string(desc), "GameConfig", LOG_INFO);
    }

    Utilities::Log(std::to_string(updateRate));
    updateTime = 1.0f / updateRate;
    configured = true;
}