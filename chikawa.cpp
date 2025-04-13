#include <raylib.h>
class Chikawa {
    public:
    Texture2D texture;
    Vector2 position = {(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() - 200};
    float scale = 0.3f;
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