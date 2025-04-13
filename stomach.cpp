
#include <raylib.h>
#include "chikawa.cpp"
//g++ helloworld.cc -o game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
//to compile
class Stomach
{
    public:
    Texture2D texture;
    Texture2D spaceShipTexture;
    Texture2D randomSpaceShipTexture;
    Vector2 position = {150, (float)GetScreenHeight()- 400};
    float scale = 0.5f;
    float jumpStrength = 10.0f;  // How high the jump is
    float gravity = 0.5f; 
    bool isJumping = false;
    float velocity = 0.0f;
    float groundLevel = 500;
    int RandomGeneratorMin()
    {
        int choices[2] = {positionSpace.x - 250, positionSpace.x + 250};
        int x = choices[GetRandomValue(0,1)];
        while (x <= 5 || x >=1170)
        {
            int x = choices[GetRandomValue(0,1)];
        }
        
        return x;
    }
    
    float randomSpaceShip = RandomGeneratorMin();
    Chikawa chikawa = Chikawa();
    bool running = true;
    Vector2 positionSpace = {45.0f, (float) GetScreenHeight()- 280.0f};
    Vector2 positionSpaceShip = {(float)randomSpaceShip, (float)chikawa.position.y - 170};
    float scaleSpace = 0.3f;
    Stomach()
    {
        Image image = LoadImage("stomach.png");
        texture = LoadTextureFromImage(image);
        Image spaceship = LoadImage("spaceship.png");
        spaceShipTexture = LoadTextureFromImage(spaceship);
        UnloadImage(image);
        UnloadImage(spaceship);
        Image randomspaceshipimage = LoadImage("spaceship.png");
        randomSpaceShipTexture = LoadTextureFromImage(randomspaceshipimage);
        UnloadImage(randomspaceshipimage);
        }
        ~Stomach()
        {
            UnloadTexture(texture);
            UnloadTexture(spaceShipTexture);
            UnloadTexture(randomSpaceShipTexture);
        }
    
        void Draw()
        {
        Rectangle source = { 0, 0, (float)texture.width, (float)texture.height }; // Full image
        Rectangle dest = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }; // Fit to screen
        Vector2 origin = { 0, 0 }; // No rotation, so origin is (0,0)    
        DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
        DrawTextureEx(spaceShipTexture, positionSpace, 0.0f,scaleSpace, WHITE);
       
        chikawa.Draw();
        }
        void Movement()
        {
            
        if (IsKeyDown(KEY_UP)) chikawa.position.y -= 5.0f;
        if (IsKeyDown(KEY_DOWN)) chikawa.position.y += 5.0f;
        if (IsKeyDown(KEY_RIGHT)) chikawa.position.x += 5.0f;
        if (IsKeyDown(KEY_LEFT)) chikawa.position.x -= 5.0f;  

    // Jumping logic
    if (IsKeyDown(KEY_SPACE) && !isJumping) 
    {
        velocity -= jumpStrength;  // Move up quickly
        isJumping = true;  // Prevent multiple jumps mid-air
    }
    
    if (isJumping) 
    {
        chikawa.position.y += velocity; // Move based on velocity
        velocity += gravity; // Gravity slows upward motion, then pulls downward

        // Stop falling when reaching the initial position
        if (chikawa.position.y >= groundLevel) // Change 500 to your starting position
        {
            chikawa.position.y = 500; // Snap to the ground
            velocity = 0.0f; // Reset velocity
            isJumping = false; // Allow jumping again
        }
    
    }

        }
        void Update()
        {
            CheckCollisionWithEdges();
            Spaceships();
        }
        void Spaceships()
        {
          DrawTextureEx(spaceShipTexture, positionSpaceShip, 0.0f,scaleSpace, WHITE);   
        }
        void CheckCollisionWithEdges()
        {
        if (chikawa.position.x <= 0)
        {
            chikawa.position.x = 1; // Keep inside screen bounds
        }
        if (chikawa.position.x >= 1170)
        {
            chikawa.position.x = 1170;
        }
    }
        void CheckCollisionWithSpaceShips()
        {
            if (chikawa.position.x == randomSpaceShip)
            {
                /* code */
            }
            
        }
        void GameOver()
        {
            //reset everything
            position.x;
        }
        
    };
    