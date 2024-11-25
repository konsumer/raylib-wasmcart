#include "raylib.h"

Texture2D texture;

int main() {
    SetWindowSize(800, 600);
    Image image = LoadImage("/logo.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    return 0;
}

void update(double time) {
    ClearBackground(RAYWHITE);
    DrawTexture(texture, 800/2 - texture.width/2, 600/2 - texture.height/2, WHITE);
    DrawText("Congrats! You created your first window!", 190, 290, 20, LIGHTGRAY);
}