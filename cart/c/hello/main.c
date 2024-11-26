#include "raylib.h"

Texture2D logo;

int main() {
  SetWindowSize(800, 600);
  Image image = LoadImage("logo.png");
  logo = LoadTextureFromImage(image);
  UnloadImage(image);
  return 0;
}

void update(double time) {
  ClearBackground(BLUE);
  DrawTexture(logo, 800/2 - logo.width/2, 600/2 - (logo.height + 20), WHITE);
  DrawText("Congrats! You created your first window!", 190, 290, 20, LIGHTGRAY);
}
