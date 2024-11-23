#include "raylib.h"
#ifdef EMSCRIPTEN
#include "null0_host_emscripten.h"
#else
#include "null0_host_wamr.h"
#endif

int main(int argc, char *argv[]) {
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(320, 240, "null0");
  SetTargetFPS(60);

  null0_host_load(argc, argv);

  while (!WindowShouldClose()) {
    BeginDrawing();
    null0_host_update(GetTime());

    // ClearBackground(RAYWHITE);
    // DrawText("OMG! it works", 100, 110, 20, BLACK);
    EndDrawing();
  }

  CloseWindow();
  null0_host_unload();
  return 0;
}