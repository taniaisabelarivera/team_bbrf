#include <raylib.h>

static Texture2D alienImages[3] = {};

class Alien {
    public:
    
    Alien(int type, Vector2 position)
    {
        this -> type = type;
        this -> position = position;

        if(alienImages[type - 1].id == 0){

        switch (type)
        {
        case 1:
            alienImages[0] = LoadTexture("alien.png");
            break;
        case 2:
        alienImages[1] = LoadTexture("spaceship.png");
        break;
        case 3:
        alienImages[2] = LoadTexture("alien.png");
        break;
        default:
        alienImages[0] = LoadTexture("alien.png");
        break;
        }
    }
}
    void Update(int direction)
    {
        position.x += direction;
    }
   ;
    void Draw()
    {
        DrawTextureEx(alienImages[type], position, 0.0f, 0.13f, WHITE);

    }
    int GetType()
    {
        return type;
    }
    Rectangle GetRect(){ 
    float scale = 0.13f;
    Texture2D tex = alienImages[type];
        return {position.x, position.y, tex.width * scale,
            tex.height * scale};
    }
    int type;
    Vector2 position;
    private:

};