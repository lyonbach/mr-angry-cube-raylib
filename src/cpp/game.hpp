#ifndef GAME_HPP
#define GAME_HPP

#include "raylib.h"
#include "../../vendor/json.hpp"
#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;

class Game {
public:
    static Game& getInstance() {
        static Game instance;
        return instance;
    }

    Game(const Game&) = delete;
    void operator=(const Game&) = delete;

    void LoadSettings(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open settings file: " << filename << std::endl;
            return;
        }
        file >> settings;
    }

    json settings;

private:
    Game() {}
    ~Game() {}
};

#endif
