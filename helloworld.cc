#include <iostream>

int main() {
    InitWindow(800, 450, "2D Fighter");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("2D Fighter Game!", 300, 200, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}