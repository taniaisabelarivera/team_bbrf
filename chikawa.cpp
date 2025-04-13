#include <raylib.h>
class Chikawa {
    public:
    Texture2D texture;
    Vector2 position = {150, (float)GetScreenHeight() - 350};
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