#include "Level.h"
#include "Utilities.h"
#include "StaticObject.h"
#include <fstream>


Level::Level(std::string levelName, Game* game)
{
    std::string filePath = "./assets/levels/" + levelName + ".lvl";
    std::ifstream levelFile(filePath);
    if (!levelFile.is_open())
    {
        Utilities::Log("Failed to open level file: " + filePath, "Level", LOG_ERROR);
        return;
    }

    // If the file is existent, log the successful opening of the file
    Utilities::Log("Successfully opened level file: " + filePath, "Level", LOG_DEBUG);
    std::string line;
    while (std::getline(levelFile, line))
    {
        // Process each line of the file
        int count = 3;
        char** tokens;
        tokens = TextSplit(line.c_str(), '|', &count);

        std::string objName = std::string(tokens[0]).c_str();
        std::string objType = std::string(tokens[2]).c_str();
        char** coords;
        coords = TextSplit(std::string(tokens[1]).c_str(), '_', &count);

        Vector3 position;
        position.x = std::stof(coords[0]);
        position.y = std::stof(coords[1]);
        position.z = std::stof(coords[2]);

        if (objType == "s")
        {
            StaticObject* object = new StaticObject(&game->models[objName], &game->materials["staticObjectDefault"], &game->textures["enemyDefault"]);
            object->SetPosition(position);
            staticObjects.push_back(object);
        } else if (objType == "d")
        {
            Utilities::Log("NOT YET IMPLEMENTED!");
        }

    }
    levelFile.close();
    loaded = true;
}

Level::~Level()
{
    Utilities::Log("Destroying " + levelName + "...", "Level");
    Game::Get().currentLevel = nullptr;
    for (auto obj : staticObjects)
    {
        delete obj;
    }
    staticObjects.clear();
}
