#include <iostream>
#include <raylib.h>

class Chikawa {
public:
Texture2D texture;
Vector2 position = {150, (float)GetScreenHeight()- 400};
float scale = 0.5f;
Chikawa()
{
    Image image = LoadImage("skin_cha_main_1.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
}
~Chikawa()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTextureEx(texture, position, 0.0f,scale, WHITE);
    }

};
int main() {
    //1280 x 720 
    const int screen_width = 1280;
    const int screen_height = 720;
    InitWindow(screen_width, screen_height, "Chikawa");
    SetTargetFPS(60);
    Chikawa chikawa = Chikawa();
    while(WindowShouldClose() == false)
    {
        BeginDrawing();
        
        ClearBackground(RED);
        chikawa.Draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
    
}