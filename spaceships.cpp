#include <raylib.h>

#include "chikawa.cpp"
#include "laser.cpp"
#include <vector>
class Spaceship
{
public:
Chikawa chikawa = Chikawa();
std::vector<Laser> lasers;


    Spaceship()
    {
        chikawa.position.x = GetScreenWidth() / 2 - 125;
        chikawa.position.y = GetScreenHeight() - 175;
    }


 
       // DrawTextureEx(Chikawa.Draw(), position, 0.0f, 0.2f,  WHITE);
    
    void MoveLeft()
    {
        chikawa.position.x -= 7;
        if(chikawa.position.x < 0)
        {
            chikawa.position.x = 0;
        }
    }
    void MoveRight()
    {
        chikawa.position.x += 7;
        if(chikawa.position.x >= GetScreenWidth() - 125)
        {
            chikawa.position.x = GetScreenWidth() - 125;
        }
    }
    void FireLaser()
    {
        lasers.push_back(Laser({chikawa.position.x + 60, chikawa.position.y},-6));
    }
    void Update()
    {
        
        chikawa.Draw();
    }
    Rectangle getRect()
{
    return {chikawa.position.x, chikawa.position.y, float(chikawa.texture.width), float(chikawa.texture.height)};
}
};
