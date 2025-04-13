#include <iostream>
#include <raylib.h>
#include "minigame_1.h"  // Include the header where Mouth is declared

// Define Fighter structure
struct Fighter {
    Vector2 position;
    Vector2 size;
    Color color;
    int speed;
    int health;
};

// Enum for Game State (Menu or Fight)
enum GameState {
    MENU,
    FIGHT
};

int main() {
    const int screenWidth = 1280;  // Screen width
    const int screenHeight = 780;  // Screen height

    // Initialize window
    InitWindow(screenWidth, screenHeight, "2D Fighter Game");
    SetTargetFPS(60);

    // Create an instance of Mouth class
    Mouth mouth;

    // Initialize Fighters
    Fighter p1 = {{100, screenHeight - 150}, {50, 100}, RED, 5, 100}; // Player 1
    Fighter p2 = {{screenWidth - 150, screenHeight - 150}, {50, 100}, BLUE, 5, 100}; // Player 2

    GameState currentScreen = MENU;  // Start at the Menu

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (currentScreen == MENU) {
            // Draw Menu Screen
            DrawText("2D STREET FIGHTER", 250, 150, 30, WHITE);
            DrawText("Press ENTER to Start", 270, 220, 20, GRAY);

            if (IsKeyPressed(KEY_ENTER)) {
                currentScreen = FIGHT;  // Move to the FIGHT screen
            }

        } else if (currentScreen == FIGHT) {
            // Draw Background using Mouth instance
            mouth.Draw();  // Call Draw method of Mouth class

            // Player 1 movement (A/D keys)
            if (IsKeyDown(KEY_A)) p1.position.x -= p1.speed;
            if (IsKeyDown(KEY_D)) p1.position.x += p1.speed;

            // Player 2 movement (Arrow keys)
            if (IsKeyDown(KEY_LEFT)) p2.position.x -= p2.speed;
            if (IsKeyDown(KEY_RIGHT)) p2.position.x += p2.speed;

            // Draw Fighters
            DrawRectangleV(p1.position, p1.size, p1.color);
            DrawRectangleV(p2.position, p2.size, p2.color);

            // Draw Health
            DrawText(TextFormat("P1 Health: %d", p1.health), 10, 10, 20, WHITE);
            DrawText(TextFormat("P2 Health: %d", p2.health), 600, 10, 20, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}