#include <raylib.h>

class Laser
{
    public:
     bool active;
    Laser(Vector2 position, int speed)
    {
        this -> position = position;
        this -> speed = speed;
        active = true;
       
    }
   
    
    void Draw()
    {   if(active)
        DrawRectangle(position.x, position.y, 4, 15, RED);
        
    }
    void Update()
    {
    position.y += speed;
    if(active)
    {
        if(position.y > GetScreenHeight() || position.y < 0)
        {
            active = false;
        }
    }
    }
    Rectangle getRect()
    {
        Rectangle rect;
        rect.x = position.x ;
        rect.y = position.y ;
        rect.width = 4.0f; 
        rect.height = 15.0f;
        return rect;
    }
    private:
    Vector2 position;
    int speed;
};