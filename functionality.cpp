#include <raylib.h>
#include "spaceships.cpp"
#include "alien.cpp"
#include "block.cpp"
#include "usagi.cpp"
class Functionality
{
public:
constexpr static float alienLaserShootInterval = 0.35;
float timeLastAlienFired;
float timeLastSpawn;
float mysteryShipSpawnInterval;
Functionality()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    alienDirection = 1;
    timeLastAlienFired = 0.0;
    timeLastSpawn = 0.0;
    mysteryShipSpawnInterval = GetRandomValue(10, 20);
    
    
    
}
~Functionality()
{
    UnloadImages();
}
void Draw()
{
    for (auto& laser : spaceship.lasers){


        laser.Draw();
    }

    for (auto& obstacle : obstacles)
    {
        obstacle.Draw();
    }

    for (auto& alien: aliens)
    {
        alien.Draw();
    }
    for (auto& laser : alienLasers)
    {
        laser.Draw();
    }
    usagi.Draw();
}

void Update()
{
    spaceship.Update();
    for (auto& laser : spaceship.lasers){
        laser.Update();
    }
    MoveAliens();
    double currentTime = GetTime();
    if (currentTime - timeLastSpawn > mysteryShipSpawnInterval)
    {
        usagi.Spawn();
        timeLastSpawn = GetTime();
        mysteryShipSpawnInterval = GetRandomValue(10, 20);
    }
    usagi.Update();
    AlienShootLaser();
    for (auto& laser : alienLasers)

    {
       
        laser.Update();
    }
    

    DeleteInactiveLasers();
    CheckForCollions();
    gameStatus();
}
void HandleInput()
{
    if(IsKeyDown(KEY_LEFT))
    {
        spaceship.MoveLeft(); 
    } else if (IsKeyDown(KEY_RIGHT))
    {
        spaceship.MoveRight();
    } else if (IsKeyDown(KEY_SPACE))
    {
        spaceship.FireLaser();
    }
}

private:
void DeleteInactiveLasers()
{
    for (auto it = spaceship.lasers.begin(); it!= spaceship.lasers.end();)
    {
        if (!it -> active)
        {
            it = spaceship.lasers.erase(it);
        } else {
            ++ it;
        }    
    }
    for (auto it = alienLasers.begin(); it!= alienLasers.end();)
    {
        if (!it -> active)
        {
            it = alienLasers.erase(it);
        } else {
            ++ it;
        }    
    }
}
Spaceship spaceship = Spaceship();
Usagi usagi = Usagi();
std::vector<Obstacle> obstacles;
std::vector<Alien> aliens;
int alienDirection;
std::vector<Alien> CreateAliens()
{
    
    for(int row = 0; row < 4; row++)
    {
        for(int column = 0; column < 10; column++)
        {   
            int alienType;
            if(row == 0)
            {
                alienType = 3;
            } else if (row == 1 || row == 2)
            {
                alienType = 2;
            } else{
                alienType = 1;
            }
            float x = 185 + column * 85;
            float y = 15 + row * 85;
            aliens.push_back(Alien(alienType, {x, y}));
        }
    }
    return aliens;
}
void UnloadImages()
{
    for(int i = 0; i < 4; i++)
    {
        UnloadTexture(alienImages[i]);
    }
}
void MoveAliens()
{
    for(auto& alien : aliens)
    {
        if(alien.position.x > GetScreenWidth() - 100)
        {
            alienDirection = -1;
            MoveDownAliens(10);
        }
        if(alien.position.x < 0)
        {
            alienDirection = 1;
            MoveDownAliens(4);
        }

        alien.Update(alienDirection);
    }
}
void MoveDownAliens(int distance)
{
    for(auto& alien : aliens)
    {
        alien.position.y += distance;
    }
}
std::vector<Obstacle> CreateObstacles()
{
    int obstacleWidth = grid[0].size() * 2;
    float gap = (GetScreenWidth() - (4 * obstacleWidth))/5;

    for (int i = 0; i < 4; i++)
    {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() - 300)}));
        
    }return obstacles;
}

std::vector<Laser> alienLasers;
void AlienShootLaser()
{
    double currentTime = GetTime();
    if(currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty())
    {
     int randomIndex = GetRandomValue(0, aliens.size() - 1);
    Alien& alien = aliens[randomIndex];
    //Vector2 pos{{aliens(x, y), aliens{x, y}}};
    alienLasers.push_back(Laser({alien.position.x + alienImages[alien.type -1].width/10 ,
        alien.position.y + alienImages[alien.type -1].height/5}, 8));
        timeLastAlienFired = GetTime();
        
}   
    }
    
void CheckForCollions()
{
    for(auto& laser : spaceship.lasers)
    {
        auto it = aliens.begin();
        while(it != aliens.end())
        {
            if(CheckCollisionRecs(it -> GetRect(), laser.getRect()))
            {
                it = aliens.erase(it);
                laser.active = false;
                
            } else {
                ++it;
            }
        }
    
        for(auto& obstacle : obstacles)
        {
        auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
              if(CheckCollisionRecs(it -> getRect(), laser.getRect()))
              {
                it = obstacle.blocks.erase(it);
                laser.active = false;
              } else {
                ++it;
              }
            }
        }
      if(CheckCollisionRecs(usagi.getRect(), laser.getRect()) )
      {
        usagi.alive = false;
        laser.active = false;
      } 
    }
    
        for(auto& laser : alienLasers)
        {
            if(CheckCollisionRecs(laser.getRect(), spaceship.getRect()))
            {
                laser.active = false; 
            }
        
        for(auto& obstacle : obstacles)
        {
        auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
              if(CheckCollisionRecs(it -> getRect(), laser.getRect()))
              {
                it = obstacle.blocks.erase(it);
                laser.active = false;
              } else {
                ++it;
              }
            }
        }}
    for(auto& alien : aliens) {
        for(auto& obstacle : obstacles){
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end())
            {
                if(CheckCollisionRecs(it -> getRect(), alien.GetRect()))
                {
                    it = obstacle.blocks.erase(it);
                } else {
                    it ++;
                }
            }
        }
        
    }
    
}void gameStatus()
    {
        if (aliens.empty()) {
            gameWon = true;
        }
        
        if (gameWon) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
    const char* text = gameOver ? "GAME OVER" : "YOU WIN!";
    int fontSize = 60;
    DrawText(text,
             GetScreenWidth()/2 - MeasureText(text, fontSize)/2,
             GetScreenHeight()/2 - fontSize/2,
             fontSize,
             gameOver ? RED : GREEN);
}

}  
bool gameOver = false;
bool gameWon = false;

};


