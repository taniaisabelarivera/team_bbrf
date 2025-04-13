
#include <iostream>
#include <raylib.h>
#include "mouth.h"  // Include the header where Mouth is declared

// Fighter structure
struct Fighter {
    Vector2 position;
    Vector2 size;
    Color color;
    int speed;
    int health;
    Texture2D texture;  // To hold the character's texture
};

// Game states
enum GameState {
    MENU,
    FIGHT,
    WIN_SCREEN
};

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 780;

    InitWindow(screenWidth, screenHeight, "2D Fighter Game");
    SetTargetFPS(60);

    Mouth mouth;

    // Load textures for the characters
    Texture2D tongueTexture = LoadTexture("tongue.png");   // Player 1 texture (tongue)
    Texture2D chikawaTexture = LoadTexture("AdorableCutieChiikawa.png"); // Player 2 texture (Chikawa)

    // Player 1 (Blue) = tongue.png, Player 2 (Red) = chikawa.png
    Fighter p1 = {{100, screenHeight - 150}, {tongueTexture.width * 0.5f, tongueTexture.height * 0.5f}, WHITE, 3, 150, tongueTexture};  // Blue slower (speed reduced to 3)
    Fighter p2 = {{screenWidth - 150, screenHeight - 250}, {chikawaTexture.width * 0.5f, chikawaTexture.height * 0.5f}, RED, 5, 100, chikawaTexture};  // Red with normal speed

    GameState currentScreen = MENU;

    // Attack cooldown (in frames, 60 frames per second)
    float attackCooldown = 1.0f;  // 1 second cooldown
    float attackTimer = 0.0f;  // Timer to keep track of cooldown time

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (currentScreen == MENU) {
            DrawText("2D STREET FIGHTER", 250, 150, 30, WHITE);
            DrawText("Press ENTER to Start", 270, 220, 20, GRAY);

            if (IsKeyPressed(KEY_ENTER)) {
                currentScreen = FIGHT;
            }

        } else if (currentScreen == FIGHT) {
            mouth.Draw();

            // Player 1 (blue) automatic movement towards Player 2 (red)
            if (p1.position.x < p2.position.x) p1.position.x += p1.speed;  // Move right if to the left
            if (p1.position.x > p2.position.x) p1.position.x -= p1.speed;  // Move left if to the right

            // Player 1 attacks automatically when close to Player 2 with cooldown
            if (attackTimer <= 0.0f) {
                // Define rectangles for collision detection (using the size of the textures for hitboxes)
                Rectangle rect1 = {p1.position.x, p1.position.y, p1.size.x, p1.size.y};  // Player 1 hitbox
                Rectangle rect2 = {p2.position.x, p2.position.y, p2.size.x, p2.size.y};  // Player 2 hitbox

                // Check for collision between Player 1 and Player 2
                if (CheckCollisionRecs(rect1, rect2)) {
                    p2.health -= 10;  // Player 1 attacks Player 2
                    attackTimer = attackCooldown;  // Reset attack timer after attack
                }
            } else {
                attackTimer -= GetFrameTime();  // Decrease attack timer by time elapsed since last frame
            }

            // Player 2 movement (controlled by arrow keys)
            if (IsKeyDown(KEY_LEFT)) p2.position.x -= p2.speed;
            if (IsKeyDown(KEY_RIGHT)) p2.position.x += p2.speed;

            // Draw Fighters using textures instead of rectangles
            DrawTextureEx(p1.texture, p1.position, 0.0f, 0.5f, WHITE);  // Draw Player 1 (blue) as the tongue image
            DrawTextureEx(p2.texture, p2.position, 0.0f, 0.5f, WHITE);  // Draw Player 2 (red) as the chikawa image

            // Draw Health
            DrawText(TextFormat("P1 Health: %d", p1.health), 10, 10, 20, WHITE);
            DrawText(TextFormat("P2 Health: %d", p2.health), 600, 10, 20, WHITE);

            // Transition to WIN screen if P2's health reaches 0
            if (p2.health <= 0) {
                currentScreen = WIN_SCREEN;
            }

        } else if (currentScreen == WIN_SCREEN) {
            DrawText("YOU DEFEATED RED!", 400, 300, 30, GREEN);
            DrawText("Press R to Restart", 420, 350, 20, WHITE);

            if (IsKeyPressed(KEY_R)) {
                p1.health = 150;  // Reset P1's health
                p2.health = 100;  // Reset P2's health
                p1.position = {100, screenHeight - 150};  // Reset positions
                p2.position = {screenWidth - 150, screenHeight - 150};
                currentScreen = MENU;
            }
        }

        EndDrawing();
    }

    // Unload textures after use
    UnloadTexture(tongueTexture);
    UnloadTexture(chikawaTexture);

    CloseWindow();
    return 0;
}
