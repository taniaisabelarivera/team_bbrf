#include <raylib.h>

class Usagi{
    public:
    bool alive;
    Usagi()
    {
        image = LoadTexture("usagi.png");
        alive = false;
    }
    ~Usagi()
    {
        UnloadTexture(image);
    }
    void Update()
    {
        if(alive){
            position.x += speed;
            if(position.x > GetScreenWidth() - image.width || position.x < 0)
            {
                alive = false;
            }
        }
    }
    void Draw()
    {
        if(alive){
            DrawTextureEx(image, position, 0.0f, 0.4f, WHITE);
        }
    }
    void Spawn()
    {
        position.y = 20;
        int side = GetRandomValue(0, 1);

        if(side == 0)
        {
            position.x = 0;
            speed = 3;
        } else {
            position.x = GetScreenWidth() - image.width;
            speed = -3;
        }
        alive = true;
    }
    Rectangle getRect()
    {
        if(alive){
            return {position.x, position.y,
            float(image.width), float(image.height)};
        } else {
            return {position.x, position.y, 0, 0};
        }
        
    }
    private:
    Vector2 position;
    Texture2D image;
    int speed;

};