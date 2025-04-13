#include "raylib.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

const int MAZE_COLS = 32;
const int MAZE_ROWS = 18;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int TILE_SIZE = SCREEN_WIDTH / MAZE_COLS;
const int NUM_COLLECTIBLES = 20; // Number of collectibles to generate
const int NUM_ENEMIES = 5;// Number of enemies to generate

enum Direction { TOP = 1, RIGHT = 2, BOTTOM = 4, LEFT = 8 };
int DX[] = {0, 1, 0, -1};
int DY[] = {-1, 0, 1, 0};
Direction OPPOSITE[] = {BOTTOM, LEFT, TOP, RIGHT};

int maze[MAZE_ROWS][MAZE_COLS];
bool visited[MAZE_ROWS][MAZE_COLS] = { false };

// --- UPDATED GameState Enum ---
enum GameState {
    GAMEPLAY,
    GAME_OVER,
    GAME_WIN // Added state
};

// Structure to represent a collectible
struct Collectible {
    Vector2 position;
    bool active;
    float animationTime;
};

// --- UPDATED Enemy Structure (for pacing) ---
struct Enemy {
    Vector2 position;
    float speed;
    float animationTime;
    Color tint;     // Tint to differentiate enemies
    float rotation; // Rotation for movement direction
    bool movingRight; // True if moving right, false if moving left
};

void generateMaze(int x, int y) {
    visited[y][x] = true;
    std::vector<int> dirs = {0, 1, 2, 3};
    std::random_shuffle(dirs.begin(), dirs.end());

    for (int i : dirs) {
        int nx = x + DX[i];
        int ny = y + DY[i];

        if (nx >= 0 && ny >= 0 && nx < MAZE_COLS && ny < MAZE_ROWS && !visited[ny][nx]) {
            maze[y][x] |= (1 << i);
            maze[ny][nx] |= OPPOSITE[i];
            generateMaze(nx, ny);
        }
    }
}

float ClampValue(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Get a random position in the center of a maze cell
Vector2 getRandomCellPosition() {
    int x = rand() % MAZE_COLS;
    int y = rand() % MAZE_ROWS;
    return { x * TILE_SIZE + TILE_SIZE / 2.0f, y * TILE_SIZE + TILE_SIZE / 2.0f };
}

// Get a random position that's not too close to the player's starting position
Vector2 getEnemyStartPosition() {
    int x, y;
    do {
        x = rand() % MAZE_COLS;
        y = rand() % MAZE_ROWS;
        // Ensure enemies start at least 1/3 of the maze away from the player
    } while (x < MAZE_COLS/3 && y < MAZE_ROWS/3);

    return { x * TILE_SIZE + TILE_SIZE / 2.0f, y * TILE_SIZE + TILE_SIZE / 2.0f };
}

// --- Enemy Pacing Movement Function ---
void moveEnemyPacing(Enemy &enemy, float delta) {
    // Determine movement direction
    float moveX = (enemy.movingRight) ? 1.0f : -1.0f;
    float moveY = 0; // No vertical movement

    // Calculate potential new position
    Vector2 newPos = enemy.position;
    newPos.x += moveX * enemy.speed * delta;
    // newPos.y remains the same

    // Check if new position would hit a wall horizontally
    int currentCellX = enemy.position.x / TILE_SIZE;
    int currentCellY = enemy.position.y / TILE_SIZE;
    int newCellX = newPos.x / TILE_SIZE;
    // Keep cell Y the same as we only move horizontally
    int newCellY = currentCellY;

    bool blocked = false;

    // Prevent division by zero or out-of-bounds access if somehow enemy gets off grid
    if (currentCellY < 0 || currentCellY >= MAZE_ROWS || currentCellX < 0 || currentCellX >= MAZE_COLS) {
         enemy.position.x = ClampValue(enemy.position.x, TILE_SIZE/2.0f, SCREEN_WIDTH - TILE_SIZE/2.0f);
         enemy.position.y = ClampValue(enemy.position.y, TILE_SIZE/2.0f, SCREEN_HEIGHT - TILE_SIZE/2.0f);
         return; // Skip movement this frame if out of bounds
    }


    // Check X movement only if crossing a cell boundary
    if (newCellX != currentCellX) {
        if (moveX > 0 && !(maze[currentCellY][currentCellX] & RIGHT)) {
             blocked = true; // Blocked moving right
        }
        if (moveX < 0 && !(maze[currentCellY][currentCellX] & LEFT)) {
             blocked = true; // Blocked moving left
        }
    }

    // Handle collision or apply movement
    if (blocked) {
        // Hit a wall, reverse horizontal direction
        enemy.movingRight = !enemy.movingRight;
    } else {
        // No wall blocking horizontal movement, update position
        enemy.position.x = newPos.x;
    }

    // Update rotation based on new movement direction
    enemy.rotation = (enemy.movingRight) ? 0.0f : 180.0f; // 0 degrees for right, 180 for left

    // Keep enemy within overall maze bounds
    enemy.position.x = ClampValue(enemy.position.x, TILE_SIZE/2.0f, SCREEN_WIDTH - TILE_SIZE/2.0f);
    enemy.position.y = ClampValue(enemy.position.y, TILE_SIZE/2.0f, SCREEN_HEIGHT - TILE_SIZE/2.0f);
}


int main() {
    srand(time(NULL));

    // Reset visited array for maze generation
    for (int y = 0; y < MAZE_ROWS; y++) {
        for (int x = 0; x < MAZE_COLS; x++) {
            visited[y][x] = false;
            maze[y][x] = 0;
        }
    }

    generateMaze(0, 0);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Stomach Maze Game");
    InitAudioDevice();
    SetTargetFPS(60);

    Music music = LoadMusicStream("youtube_LzzW_TBL558_audio.mp3");

    PlayMusicStream(music);

    // Load textures
    Texture2D background = LoadTexture("stomach.png");
    Texture2D playerSprite = LoadTexture("skin_cha_main_1.png");
    Texture2D collectibleSprite = LoadTexture("collectible.png");
    Texture2D enemySprite = LoadTexture("enemy.png");

    // Game state variables
    GameState currentState = GAMEPLAY;
    int score = 0;
    float gameOverTimer = 0;
    // Optional: float winTimer = 0; // If needed for win screen animations

    // Player settings
    Vector2 playerPos = { TILE_SIZE / 2.0f, TILE_SIZE / 2.0f };
    float playerSpeed = 6.0f;
    float playerRadius = 12.0f;
    float playerScale = 0.10f;

    // Enemy settings (Using Pacing Enemy structure)
    float enemyScale = 0.10f;
    float enemyRadius = 5.0f; // Keep enemy collision radius

    // Initialize collectibles
    std::vector<Collectible> collectibles;
    for (int i = 0; i < NUM_COLLECTIBLES; i++) {
        Collectible c;
        c.position = getRandomCellPosition();
        c.active = true;
        c.animationTime = (float)(rand() % 100) / 100.0f;
        collectibles.push_back(c);
    }

    // Initialize enemies (Using Pacing Enemy structure)
    std::vector<Enemy> enemies;
    Color enemyTints[] = {
        {255, 50, 50, 255},    // Red tint
        {50, 255, 50, 255},    // Green tint
        {50, 50, 255, 255},    // Blue tint
        {255, 255, 50, 255},   // Yellow tint
        {255, 50, 255, 255}    // Purple tint
    };

    for (int i = 0; i < NUM_ENEMIES; i++) {
        Enemy e;
        e.position = getEnemyStartPosition();
        // --- Adjust Speed Here If Needed ---
        e.speed = 60.0f + ((float)rand() / RAND_MAX) * 60.0f;  
        // --- End Speed Adjustment ---
        e.animationTime = (float)(rand() % 100) / 100.0f;
        e.tint = enemyTints[i % 5];
        e.movingRight = (rand() % 2 == 0); // Initialize pacing direction
        e.rotation = (e.movingRight) ? 0.0f : 180.0f; // Set initial rotation
        enemies.push_back(e);
    }

    // Define bright pink color for the walls
    Color pink = { 255, 105, 180, 255 }; // Hot Pink
    float wallThickness = 6.0f; // Wall thickness

    // --- Main Game Loop ---
    while (!WindowShouldClose()) {
        UpdateMusicStream(music);
        float deltaTime = GetFrameTime(); // deltaTime is declared here

        // --- UPDATE LOGIC based on GameState ---
        if (currentState == GAMEPLAY) {
            // --- Player Input & Movement ---
            Vector2 oldPos = playerPos;
            Vector2 moveDir = {0, 0};
            if (IsKeyDown(KEY_W)) moveDir.y -= 1;
            if (IsKeyDown(KEY_S)) moveDir.y += 1;
            if (IsKeyDown(KEY_A)) moveDir.x -= 1;
            if (IsKeyDown(KEY_D)) moveDir.x += 1;

            // Normalize diagonal movement
            if (moveDir.x != 0 && moveDir.y != 0) {
                 float length = sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
                 moveDir.x = (moveDir.x / length) * playerSpeed;
                 moveDir.y = (moveDir.y / length) * playerSpeed;
            } else {
                 moveDir.x *= playerSpeed;
                 moveDir.y *= playerSpeed;
            }
            // Apply movement (scaled by delta time for frame rate independence)
            playerPos.x += moveDir.x * deltaTime * 60.0f; // Multiply by 60 to adjust speed if needed
            playerPos.y += moveDir.y * deltaTime * 60.0f;

            // --- Player Collision with maze walls ---
             int cx = (int)(playerPos.x / TILE_SIZE);
             int cy = (int)(playerPos.y / TILE_SIZE);

             // Basic boundary check first
             if (playerPos.x - playerRadius < 0 || playerPos.x + playerRadius > SCREEN_WIDTH ||
                 playerPos.y - playerRadius < 0 || playerPos.y + playerRadius > SCREEN_HEIGHT) {
                 playerPos = oldPos; // Revert if hitting outer screen bounds
             } else if (cx >= 0 && cy >= 0 && cx < MAZE_COLS && cy < MAZE_ROWS) {
                 // Wall collision checks (simplified - potentially revert axis independently)
                Vector2 checkPos = playerPos;

                // Check X collision based on intended movement direction
                if (moveDir.x > 0 && !(maze[cy][cx] & RIGHT) && (int)((playerPos.x + playerRadius)/TILE_SIZE) != cx) {
                    checkPos.x = oldPos.x; // Block X movement
                } else if (moveDir.x < 0 && !(maze[cy][cx] & LEFT) && (int)((playerPos.x - playerRadius)/TILE_SIZE) != cx) {
                     checkPos.x = oldPos.x; // Block X movement
                }

                 // Check Y collision based on intended movement direction
                 if (moveDir.y > 0 && !(maze[cy][cx] & BOTTOM) && (int)((playerPos.y + playerRadius)/TILE_SIZE) != cy) {
                     checkPos.y = oldPos.y; // Block Y movement
                 } else if (moveDir.y < 0 && !(maze[cy][cx] & TOP) && (int)((playerPos.y - playerRadius)/TILE_SIZE) != cy) {
                    checkPos.y = oldPos.y; // Block Y movement
                 }
                 playerPos = checkPos; // Apply potentially restricted position


             } else {
                 playerPos = oldPos; // Player outside maze grid, revert
             }
              // Final clamp just in case
             playerPos.x = ClampValue(playerPos.x, playerRadius, SCREEN_WIDTH - playerRadius);
             playerPos.y = ClampValue(playerPos.y, playerRadius, SCREEN_HEIGHT - playerRadius);


            // --- Enemy Movement (Pacing) ---
            for (auto& enemy : enemies) {
                moveEnemyPacing(enemy, deltaTime); // Use pacing function
                enemy.animationTime += deltaTime * 3.0f;
                if (enemy.animationTime > 2.0f * PI) enemy.animationTime -= 2.0f * PI;

                // Check collision with player
                float dx = playerPos.x - enemy.position.x;
                float dy = playerPos.y - enemy.position.y;
                float distance = sqrt(dx*dx + dy*dy);

                if (distance < playerRadius + enemyRadius) { // Enemy catches player
                    currentState = GAME_OVER;
                    gameOverTimer = 0;
                    break; // Exit enemy loop immediately on game over
                }
            }

            // --- Collectible Collision & Win Check ---
            if (currentState == GAMEPLAY) { // Only check if still playing
                float collectRadius = 20.0f;
                for (auto& collectible : collectibles) {
                    if (collectible.active) {
                        // Check collision between player and collectible
                        float dx = playerPos.x - collectible.position.x;
                        float dy = playerPos.y - collectible.position.y;
                        float distance = sqrt(dx*dx + dy*dy);

                        if (distance < collectRadius + playerRadius) {
                            collectible.active = false;
                            score++;
                        }
                        // Update animation time
                        collectible.animationTime += deltaTime * 2.0f;
                        if (collectible.animationTime > 2.0f * PI)
                            collectible.animationTime -= 2.0f * PI;
                    }
                }

                // Check if all collectibles are collected
                bool allCollected = true;
                for (const auto& collectible : collectibles) {
                    if (collectible.active) {
                        allCollected = false;
                        break;
                    }
                }

                // --- UPDATED: Trigger Win State ---
                if (allCollected) {
                    currentState = GAME_WIN;
                    // Optional: winTimer = 0;
                }
             } // End of check block within GAMEPLAY

        // --- GAME OVER State Update Logic ---
        } else if (currentState == GAME_OVER) {
            gameOverTimer += deltaTime; // Use deltaTime here
            // Restart the game after pressing space
            if (IsKeyPressed(KEY_SPACE)) {
                // Reset game state variables
                score = 0;
                playerPos = { TILE_SIZE / 2.0f, TILE_SIZE / 2.0f };
                // Reset collectibles
                collectibles.clear();
                for (int i = 0; i < NUM_COLLECTIBLES; i++) { Collectible c = {getRandomCellPosition(), true, (float)(rand() % 100) / 100.0f}; collectibles.push_back(c); }
                // Reset enemies
                enemies.clear();
                for (int i = 0; i < NUM_ENEMIES; i++) { Enemy e = {getEnemyStartPosition(),60.0f + ((float)rand() / RAND_MAX) *60.0f, (float)(rand() % 100) / 100.0f, enemyTints[i % 5], 0.0f, (rand() % 2 == 0)}; e.rotation = (e.movingRight) ? 0.0f : 180.0f; enemies.push_back(e); }
                currentState = GAMEPLAY;
            }

        // --- GAME WIN State Update Logic ---
        } else if (currentState == GAME_WIN) {
            // Optional: winTimer += deltaTime;
            // Check for restart input
            if (IsKeyPressed(KEY_SPACE)) {
                // Reset game state variables (same as game over reset)
                score = 0;
                playerPos = { TILE_SIZE / 2.0f, TILE_SIZE / 2.0f };
                 // Reset collectibles
                collectibles.clear();
                for (int i = 0; i < NUM_COLLECTIBLES; i++) { Collectible c = {getRandomCellPosition(), true, (float)(rand() % 100) / 100.0f}; collectibles.push_back(c); }
                // Reset enemies
                enemies.clear();
                for (int i = 0; i < NUM_ENEMIES; i++) { Enemy e = {getEnemyStartPosition(), 60.0f + ((float)rand() / RAND_MAX) * 60.0f, (float)(rand() % 100) / 100.0f, enemyTints[i % 5], 0.0f, (rand() % 2 == 0)}; e.rotation = (e.movingRight) ? 0.0f : 180.0f; enemies.push_back(e); }
                currentState = GAMEPLAY; // Go back to gameplay
            }
        } // End of state update if/else chain

        // --- Drawing ---
        BeginDrawing();
        ClearBackground(BLACK);

        // Background
        DrawTexturePro(background, {0, 0, (float)background.width, (float)background.height}, {0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT}, {0, 0}, 0.0f, WHITE);

        // Draw maze walls
        for (int y = 0; y < MAZE_ROWS; y++) {
            for (int x = 0; x < MAZE_COLS; x++) {
                float px = x * TILE_SIZE; float py = y * TILE_SIZE; int cell = maze[y][x];
                if (!(cell & TOP)) DrawLineEx({px, py}, {px + TILE_SIZE, py}, wallThickness, pink);
                if (!(cell & RIGHT)) DrawLineEx({px + TILE_SIZE, py}, {px + TILE_SIZE, py + TILE_SIZE}, wallThickness, pink);
                if (!(cell & BOTTOM)) DrawLineEx({px, py + TILE_SIZE}, {px + TILE_SIZE, py + TILE_SIZE}, wallThickness, pink);
                if (!(cell & LEFT)) DrawLineEx({px, py}, {px, py + TILE_SIZE}, wallThickness, pink);
            }
        }

        // Draw collectibles
        for (const auto& collectible : collectibles) {
            if (collectible.active) {
                float collectibleScale = 0.06f; float collectibleWidth = collectibleSprite.width * collectibleScale; float collectibleHeight = collectibleSprite.height * collectibleScale;
                float yOffset = sin(collectible.animationTime) * 3.0f;
                DrawTexturePro(collectibleSprite, {0, 0, (float)collectibleSprite.width, (float)collectibleSprite.height}, {collectible.position.x - collectibleWidth / 2, collectible.position.y - collectibleHeight / 2 + yOffset, collectibleWidth, collectibleHeight}, {0, 0}, 0.0f, WHITE);
            }
        }

        // Draw enemies (Pacing)
        for (const auto& enemy : enemies) {
            float scale = enemyScale * (1.0f + sin(enemy.animationTime) * 0.1f); float enemyWidth = enemySprite.width * scale; float enemyHeight = enemySprite.height * scale;
            DrawTexturePro(enemySprite, {0, 0, (float)enemySprite.width, (float)enemySprite.height}, {enemy.position.x, enemy.position.y, enemyWidth, enemyHeight}, {enemyWidth / 2, enemyHeight / 2}, enemy.rotation, enemy.tint);
        }

        // Draw player
        float playerWidth = playerSprite.width * playerScale; float playerHeight = playerSprite.height * playerScale;
        DrawTexturePro(playerSprite, {0, 0, (float)playerSprite.width, (float)playerSprite.height}, {playerPos.x, playerPos.y, playerWidth, playerHeight}, {playerWidth / 2, playerHeight / 2}, 0.0f, WHITE);

        // Draw score HUD
        DrawText(TextFormat("SCORE: %d", score), 20, 20, 30, WHITE);

        // --- Draw GAME OVER or GAME WIN overlays ---
        if (currentState == GAME_OVER) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(RED, 0.7f));
            const char* gameOverText = "GAME OVER!"; int fontSize = 60; int textWidth = MeasureText(gameOverText, fontSize);
            DrawText(gameOverText, SCREEN_WIDTH/2 - textWidth/2, SCREEN_HEIGHT/3, fontSize, BLACK);
            std::string scoreText = "Final Score: " + std::to_string(score); int scoreTextWidth = MeasureText(scoreText.c_str(), 40);
            DrawText(scoreText.c_str(), SCREEN_WIDTH/2 - scoreTextWidth/2, SCREEN_HEIGHT/2, 40, WHITE);
            if ((int)(gameOverTimer * 2) % 2 == 0) { const char* restartText = "Press SPACE to restart"; int restartTextWidth = MeasureText(restartText, 30); DrawText(restartText, SCREEN_WIDTH/2 - restartTextWidth/2, SCREEN_HEIGHT*2/3, 30, YELLOW); }

        } else if (currentState == GAME_WIN) { // Draw Win Screen
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(DARKBLUE, 0.8f)); // Win overlay
            const char* winText = "YOU WIN!"; int winFontSize = 70; int winTextWidth = MeasureText(winText, winFontSize);
            DrawText(winText, SCREEN_WIDTH/2 - winTextWidth/2, SCREEN_HEIGHT/3, winFontSize, GOLD);
            std::string scoreText = "Final Score: " + std::to_string(score); int scoreFontSize = 40; int scoreTextWidth = MeasureText(scoreText.c_str(), scoreFontSize);
            DrawText(scoreText.c_str(), SCREEN_WIDTH/2 - scoreTextWidth/2, SCREEN_HEIGHT/2, scoreFontSize, WHITE);
            const char* restartText = "Press SPACE to play again"; int restartFontSize = 30; int restartTextWidth = MeasureText(restartText, restartFontSize);
            DrawText(restartText, SCREEN_WIDTH/2 - restartTextWidth/2, SCREEN_HEIGHT*2/3, restartFontSize, YELLOW);
        } // End drawing overlays

        EndDrawing();
    } // --- End Main Game Loop ---

    // Cleanup
    UnloadMusicStream(music);
    CloseAudioDevice();
    UnloadTexture(background);
    UnloadTexture(playerSprite);
    UnloadTexture(collectibleSprite);
    UnloadTexture(enemySprite);
    CloseWindow();
    return 0;
} // --- End main() ---