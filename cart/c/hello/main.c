#include "raylib.h"

int main() {
    SetWindowSize(800, 600);
    return 0;
}

void update(double time) {
    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first window!", 190, 290, 20, LIGHTGRAY);
}