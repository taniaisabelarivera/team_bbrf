#pragma once
#ifndef CHIKAWA_H
#define CHIKAWA_H
#include <raylib.h>

class Chikawa {
public:
    Texture2D texture;
    Vector2 position;
    float scale;

    Chikawa(Vector2 pos, float sc = 1.0f) {
        texture = LoadTexture("AdorableCutieChiikawa.png"); // make sure this image exists
        position = pos;
        scale = sc;
    }

    ~Chikawa() {
        UnloadTexture(texture);
    }

    void Draw() {
        DrawTextureEx(texture, position, 0.0f, scale, WHITE);
    }

    void Move(float dx) {
        position.x += dx;
    }
};

#endif