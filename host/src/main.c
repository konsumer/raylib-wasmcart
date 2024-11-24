#include "null0_host.h"

int main(int argc, char *argv[]) {
  SetTraceLogLevel(LOG_WARNING);

  if (argc != 2) {
      TraceLog(LOG_ERROR, "Usage: %s <CART_FILE>", argv[0]);
      return 1;
  }

  unsigned int zipBytesSize = file_size_real(argv[1]);
  unsigned char* zipBytes = malloc(zipBytesSize);
  file_read_real(zipBytes, argv[1]);

  if (!PHYSFS_init("/")) {
    TraceLog(LOG_ERROR, "Could not init filesystem.\n");
    return 1;
  }

  if (!PHYSFS_mountMemory (zipBytes, zipBytesSize, NULL, "null0", NULL, 1)) {
    PHYSFS_deinit();
    TraceLog(LOG_ERROR, "Could not mount filesystem from %s.", argv[1]);
    return 1;
  }

  PHYSFS_sint64 wasmBytesLen = 0;
  unsigned char* wasmBytes = read_physfs_file("main.wasm", &wasmBytesLen);

  null0_host_load(wasmBytes, (int)wasmBytesLen);

  InitWindow(320, 240, "null0");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    null0_host_update(GetTime());
    EndDrawing();
  }

  CloseWindow();
  null0_host_unload();
  return 0;
}