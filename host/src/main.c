#include <raylib.h>
#include "raylib-physfs.h"
#include "file_detect.h"

// implement these in your host:

// MEMORY

// copy a pointer from cart to host
void* cart_get_pointer(unsigned int cartPtr, unsigned int len);

// copy a pointer from host to cart
unsigned int cart_set_pointer(void* hostPtr, unsigned int len);

// copy a string from cart to host
char* cart_get_string(unsigned int cartPtr);

// copy a string from host to cart
unsigned int cart_set_string(char* hostPtr);

// allocate cart-memory from host C
unsigned int cart_malloc(int size);

// free cart-memory from host C
void cart_free (unsigned int ptr);

// LIFECYCLE

// called on cart update
void null0_host_update(double timeMs);

// called on cart unload
void null0_host_unload();

#ifdef EMSCRIPTEN
#include "null0_host_emscripten.h"
#else
#include "null0_host_wamr.h"
#endif

int main(int argc, char *argv[]) {
  if (argc != 2) {
    TraceLog(LOG_ERROR, "Usage: %s <CART>", argv[0]);
    return 1;
  }

  InitWindow(320, 240, "raylib");

  DetectFileType cartType = detect_real_file_type(argv[1]);

  /*
  This handles these cases:

  zip - primary usecase: mount zip, use  main.wasm in zip as main
  dir/ - mount dir, use main.wasm in dir as main
  whatever.wasm - mount dir it's in, use whatever.wasm as main
  */

  char* wasmFilename = "main.wasm";

  if (cartType == FILE_TYPE_DIR || cartType == FILE_TYPE_ZIP) {
    InitFS(argv[1]);
  } else if (cartType == FILE_TYPE_WASM) {
    char* d = GetDirectoryPath(argv[1]);
    wasmFilename = GetFileName(argv[1]);
    TraceLog(LOG_INFO, "FS: Mounting directory of wasm-file: %s", d);
    InitFS(d);
  } else {
    TraceLog(LOG_ERROR, "FS: Cart should be a dir/wasm/zip file.");
    CloseWindow();
    return 1;
  }

  if (!FileExistsFS(wasmFilename)) {
    TraceLog(LOG_ERROR, "FS: Cart should contain a valid %s. It is the wasm entry-point.", wasmFilename);
    CloseWindow();
    CloseFS();
    return 1;
  }

  int wasmBytesLen=0;
  unsigned char* wasmBytes = LoadFileData(wasmFilename, &wasmBytesLen);

  unsigned int magic_number = 0;
  memcpy(&magic_number, wasmBytes, sizeof(unsigned int));
  if (wasmBytesLen == 0 || parse_magic_bytes(magic_number) != FILE_TYPE_WASM) {
    TraceLog(LOG_ERROR, "FS: Cart should contain a valid %s. It is the wasm entry-point.", wasmFilename);
    CloseWindow();
    CloseFS();
    return 1;
  }

  TraceLog(LOG_INFO, "FS: Entry point looks good %s: %d", wasmFilename, wasmBytesLen);
  null0_host_load(wasmBytes, wasmBytesLen);
  
  while (!WindowShouldClose()) {
    BeginDrawing();
    null0_host_update(GetTime());
    EndDrawing();
  }

  CloseWindow();
  null0_host_unload();
  CloseFS();
  return 0;
}
