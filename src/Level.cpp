#include "Level.h"
#include "Utilities.h"


Level::Level(std::string filePath)
{
    Utilities::Log("Initializing level...", "Level");
    // FIXME TEMPORARY
    for (size_t i = 0; i < 100; i+=2)
    {
        staticObjects.push_back(MatrixTranslate(5, i, 0));
        staticObjects.push_back(MatrixTranslate(5, i, -10));
        staticObjects.push_back(MatrixTranslate(-5, i, 0));
        staticObjects.push_back(MatrixTranslate(-5, i, -10));

    }
    // FIXME TEMPORARY
}


Level::~Level()
{
    Utilities::Log("Destroying level...", "Level");
}

