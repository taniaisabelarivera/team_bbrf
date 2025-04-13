#include "raylib.h"
#include <vector>
#include <string>
#include <cstdlib> // For system function

// Credits screen
void ShowCreditsScreen(int screenWidth, int screenHeight) {
    bool viewingCredits = true;
    Texture2D background = LoadTexture("bg_start.png");

    // Wait until input is released
    while (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        BeginDrawing();
        DrawTexture(background, 0, 0, WHITE);
        EndDrawing();
    }

    while (viewingCredits && !WindowShouldClose()) {
        if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            viewingCredits = false;
        }

        BeginDrawing();
        DrawTexture(background, 0, 0, WHITE);

        int headingSize = 36;
        int bodySize = 22;
        int hintSize = 18;

        DrawText("Credits", screenWidth / 2 - MeasureText("Credits", headingSize) / 2, 100, headingSize, SKYBLUE);
        DrawText("Made by Team BBRF!", screenWidth / 2 - MeasureText("Made by Team BBRF!", bodySize) / 2, 150, bodySize, WHITE);
        DrawText("Hydrocloric Acid Escape and titlescreen by Tania Rivera", screenWidth / 2 - MeasureText("Hydrochloric Acid Escape and titlescreen by Tania Rivera", bodySize) / 2, 175, bodySize, WHITE);
        DrawText("Esophagus Invaders by Lizbeth Arcos", screenWidth / 2 - MeasureText("Esophagus Invaders by Lizbeth Arcos", bodySize) / 2, 200, bodySize, WHITE);
        DrawText("Design and Assets by Jimena Hernandez", screenWidth / 2 - MeasureText("Design and Assets by Jimena Hernandez", bodySize) / 2, 225, bodySize, WHITE);
        DrawText("Royal Advising and Debugging by Jayden Chico", screenWidth / 2 - MeasureText("Royal Advising and Debugging by Jayden Chico", bodySize) / 2, 250, bodySize, WHITE);
        DrawText("Press [ENTER] or click to return to menu",
                 screenWidth / 2 - MeasureText("Press [ENTER] or click to return to menu", hintSize) / 2,
                 300, hintSize, GRAY);

        EndDrawing();
    }
    
    UnloadTexture(background);
}

// Function to launch a game executable
void LaunchGame(const char* executablePath, Texture2D& background) {
    // Close the current window before launching the game
    CloseWindow();
    
    // Launch the specified executable and get return code
    int result = 0;
    #ifdef _WIN32
        result = system(executablePath);
    #else
        result = system(executablePath);
    #endif
    
    // Re-initialize the window when returned from the game
    InitWindow(1280, 720, "Minigame Menu");
    SetTargetFPS(60);
    
    // Reload the background texture
    background = LoadTexture("bg_start.png");
}

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Minigame Menu");
    SetTargetFPS(60);
    Texture2D background = LoadTexture("bg_start.png");

    std::vector<std::string> menuItems = { "Hydrocloric Acid Maze", "Esophagus Invaders", "Credits", "Exit" };
    
    // Paths to game executables
    const char* stomachGamePath = "./stomach"; // For Linux/Mac
    // const char* stomachGamePath = "stomach.exe"; // For Windows
    
    const char* esophagusGamePath = "./esophagus"; // Placeholder for second game
    // const char* esophagusGamePath = "esophagus.exe"; // For Windows

    float baseY = 200.0f;
    float spacing = 70.0f;
    int fontSize = 24;

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();

        BeginDrawing();
        DrawTexture(background, 0, 0, WHITE);

        // Draw title
        const char* titleText = "Chiikawa: Star Swallowed!";
        int titleFontSize = 40;
        int titleWidth = MeasureText(titleText, titleFontSize);
        DrawText(titleText, screenWidth / 2 - titleWidth / 2, 80, titleFontSize, WHITE);

        // Draw buttons
        for (int i = 0; i < menuItems.size(); i++) {
            float btnWidth = 300.0f;
            float btnHeight = 50.0f;
            float btnX = screenWidth / 2 - btnWidth / 2;
            float btnY = baseY + i * spacing;

            Rectangle btn = { btnX, btnY, btnWidth, btnHeight };

            bool hovering = CheckCollisionPointRec(mousePos, btn);
            Color btnColor = hovering ? WHITE : ORANGE;
            Color textColor = hovering ? GOLD : WHITE;

            DrawRectangleRounded(btn, 0.2f, 10, btnColor);

            const char* label = menuItems[i].c_str();
            int textWidth = MeasureText(label, fontSize);
            float textX = btnX + (btnWidth - textWidth) / 2;
            float textY = btnY + (btnHeight - fontSize) / 2;

            DrawText(label, (int)textX, (int)textY, fontSize, textColor);

            // Handle click
            if (hovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (menuItems[i] == "Exit") {
                    CloseWindow();
                    return 0;
                } else if (menuItems[i] == "Credits") {
                    ShowCreditsScreen(screenWidth, screenHeight);
                } else if (menuItems[i] == "Hydrocloric Acid Maze") {
                    LaunchGame(stomachGamePath, background);
                } else if (menuItems[i] == "Esophagus Invaders") {
                    LaunchGame(esophagusGamePath, background);
                }
            }
        }

        EndDrawing();
    }

    UnloadTexture(background);
    CloseWindow();
    return 0;
}