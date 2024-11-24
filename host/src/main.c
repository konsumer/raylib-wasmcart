#define RAYLIB_PHYSFS_IMPLEMENTATION

#include "raylib.h"
#include <stdlib.h>
#include "raylib-physfs.h"

// IMPLEMENT THESE IN YOUR HOST

// called on cart init
void null0_host_load(unsigned char* wasmBytes, int wasmBytesLen);

// called on cart update
void null0_host_update(double timeMs);

// called on cart unload
void null0_host_unload();

// read a real file from fs
unsigned char* file_read_real(char* filename, unsigned int* fileSize);

#ifdef EMSCRIPTEN
#include "null0_host_emscripten.h"
#else
#include "null0_host_wamr.h"
#endif


int main(int argc, char *argv[]) {
  if (argc != 2) {
      TraceLog(LOG_ERROR, "Usage: %s <CART_FILE>", argv[0]);
      return 1;
  }

  if (!PHYSFS_init("/")) {
    TraceLog(LOG_ERROR, "Could not init filesystem.");
    return 1;
  }
  
  if (!MountPhysFS(argv[1], "/")) {
    TraceLog(LOG_ERROR, "Could not mount filesystem from %s.", argv[1]);
    ClosePhysFS();
    return 1;
  }

  int wasmBytesLen = 0;
  unsigned char* wasmBytes = LoadFileDataFromPhysFS("/main.wasm", &wasmBytesLen);

  null0_host_load(wasmBytes, wasmBytesLen);
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    null0_host_update(GetTime());
    EndDrawing();
  }

  CloseWindow();
  null0_host_unload();
  ClosePhysFS();
  
  return 0;
}
