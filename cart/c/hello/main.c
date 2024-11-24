#include "raylib.h"

int main() {
    InitWindow(800, 600, "raylib in wasm");
    return 0;
}

void update(double time) {
    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first window!", 190, 290, 20, LIGHTGRAY);
}