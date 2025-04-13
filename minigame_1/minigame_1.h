class Mouth {
public:
    Texture2D texture;
    Vector2 position = {150, (float)GetScreenHeight() - 400};
    float scale = 0.5f;

    // Constructor
    Mouth() {
        Image image = LoadImage("1__MxM_7tmnC_IhNxYK0DYxg(1).jpg");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    // Destructor
    ~Mouth() {
        UnloadTexture(texture);
    }

    // Draw function
    void Draw() {
        Rectangle source = { 0, 0, (float)texture.width, (float)texture.height }; // Full image
        Rectangle dest = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }; // Fit to screen
        Vector2 origin = { 0, 0 }; // No rotation, so origin is (0,0)
        DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
    }
};