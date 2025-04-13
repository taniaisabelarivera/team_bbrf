#include <iostream>
#include <raylib.h>
#include "stomach.cpp"

int main() {
    //1280 x 720 
    const int screen_width = 1280;
    const int screen_height = 720;
    InitWindow(screen_width, screen_height, "Chikawa");
    SetTargetFPS(60);
    Chikawa chikawa = Chikawa();
    Stomach stomach = Stomach();
    while(WindowShouldClose() == false)
    {
        BeginDrawing();
        
        ClearBackground(RED);
       
        stomach.Draw();
        stomach.Movement();
        stomach.CheckCollisionWithEdges();
        //chikawa.Draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
    
}