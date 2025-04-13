
#include <raylib.h>
#include "functionality.cpp"

//g++ helloworld.cc -o game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
//to compile
class Stomach
{
    public:
    Texture2D texture;
    Vector2 position = {150, (float)GetScreenHeight()- 400};
    Spaceship spaceship = Spaceship();
    Functionality game = Functionality();
   // Alien alien = Alien(2, {100, 100});
    //Laser laser = Laser({100, 100}, 7);
    //Obstacle obstacle = Obstacle({100, 100});
    Stomach()
    {
        Image image = LoadImage("stomach.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }
        ~Stomach()
        {
            UnloadTexture(texture);
        }
        
        void Draw()
        {
        Rectangle source = { 0, 0, (float)texture.width, (float)texture.height }; // Full image
        Rectangle dest = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }; // Fit to screen
        Vector2 origin = { 0, 0 }; // No rotation, so origin is (0,0)    
        DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
        }
        void Update()
        {
            
            game.Update();
            game.HandleInput();
            game.Draw();
           // alien.Draw();
           // obstacle.Draw();
            //laser.Update();
        }
    };

    