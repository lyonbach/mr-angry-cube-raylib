#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "raylib.h"

class Animation {
public:
    Animation(Shader shader, Texture2D texture, int frameCount);
    ~Animation();
    void Update();

private:
    Shader shader;
    Texture2D texture;
    int frameCount;
    int currentFrame;
};

#endif
