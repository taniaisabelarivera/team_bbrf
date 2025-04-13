#pragma once
#ifndef TONGUE_H
#define TONGUE_H

#include <raylib.h>

class Tongue {
public:
    Texture2D texture;
    Vector2 position;
    float scale;

    Tongue(Vector2 pos, float sc = 1.0f) {
        texture = LoadTexture("tongue_sprite.png"); // make sure this image exists
        position = pos;
        scale = sc;
    }

    ~Tongue() {
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
