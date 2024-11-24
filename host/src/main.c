#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include <libgen.h>
#include <sys/stat.h>
#include <unistd.h>

#define RAYLIB_PHYSFS_IMPLEMENTATION
#include "raylib-physfs.h"

typedef enum {
    FILE_TYPE_UNKNOWN,
    FILE_TYPE_ZIP,
    FILE_TYPE_WASM,
    FILE_TYPE_PNG,
    FILE_TYPE_JPEG,
    FILE_TYPE_WAV,
    FILE_TYPE_OGG,
    FILE_TYPE_MP3
} DetectFileType;

DetectFileType detect_file_type(uint32_t magic_number) {
  switch (magic_number) {
    case 0x4034b50:  // ZIP
        return FILE_TYPE_ZIP;
        
    case 0x6d736100:  // WASM
        return FILE_TYPE_WASM;
        
    case 0x89504E47:  // PNG
        return FILE_TYPE_PNG;
        
    // JPEG variants
    case 0xe0ffd8ff:  // JPEG/JFIF
    case 0xe1ffd8ff:  // JPEG/Exif
    case 0xeeffd8ff:  // JPEG/SPIFF
    case 0xfeffd8ff:  // JPEG/COM
        return FILE_TYPE_JPEG;
        
    case 0x46464952:  // WAV ("RIFF")
        return FILE_TYPE_WAV;
        
    case 0x5367674f:  // OGG ("OggS")
        return FILE_TYPE_OGG;
        
    // MP3 variants
    case 0x03334449:  // MP3 with ID3v2.3
    case 0x02334449:  // MP3s with ID3v2.2
    case 0x04334449:  // MP3 with ID3v2.4
        return FILE_TYPE_MP3;
        
    default:
        return FILE_TYPE_UNKNOWN;
  }
}

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
  
  if (!InitPhysFS()) {
    TraceLog(LOG_ERROR, "Could not init filesystem.");
    return 1;
  }

  struct stat sb;
  if (stat(argv[1], &sb) != 0){
    TraceLog(LOG_ERROR, "Could not open %s", argv[1]);
    ClosePhysFS();
    return 1;
  }
  bool is_dir = S_ISDIR(sb.st_mode);

  FILE* file;
  uint32_t magic_number = 0;
  DetectFileType fileType = FILE_TYPE_UNKNOWN;

  if (!is_dir) {
    file = fopen(argv[1], "rb");
    if (!file) {
      TraceLog(LOG_ERROR, "Could not open %s", argv[1]);
      ClosePhysFS();
      return 1;
    }
    fread(&magic_number, sizeof(uint32_t), 1, file);
    fclose(file);
    fileType = detect_file_type(magic_number);
  }

  unsigned int wasmBytesLen = 0;
  unsigned char* wasmBytes;

  if (is_dir || fileType == FILE_TYPE_ZIP) {
    if (!MountPhysFS(argv[1], "/")) {
      TraceLog(LOG_ERROR, "Could not mount filesystem from %s", argv[1]);
      ClosePhysFS();
      return 1;
    }
    if (!FileExistsInPhysFS("/main.wasm")) {
      TraceLog(LOG_ERROR, "Invalid cart. Must have main.wasm.");
      ClosePhysFS();
      return 1;
    }
    wasmBytes = LoadFileDataFromPhysFS("/main.wasm", &wasmBytesLen);
    if (wasmBytesLen == 0) {
      TraceLog(LOG_ERROR, "Could not read main.wasm.");
      ClosePhysFS();
      return 1;
    }
  } else if (fileType == FILE_TYPE_WASM) {
    char* dir = dirname(strdup(argv[1]));
    if (!MountPhysFS(dir, "/")) {
      TraceLog(LOG_ERROR, "Could not mount filesystem from %s", dir);
      ClosePhysFS();
      return 1;
    }
    char* wasmFilename = basename(argv[1]);

    if (!FileExistsInPhysFS(wasmFilename)) {
      TraceLog(LOG_ERROR, "Invalid cart. Must have main.wasm.");
      ClosePhysFS();
      return 1;
    }
    wasmBytes = LoadFileDataFromPhysFS(wasmFilename, &wasmBytesLen);
    if (wasmBytesLen == 0) {
      TraceLog(LOG_ERROR, "Could not read main.wasm.");
      ClosePhysFS();
      return 1;
    }
  } else {
    TraceLog(LOG_ERROR, "Cart must be a dir, zip or wasm file: %s", argv[1]);
    ClosePhysFS();
    return 1;
  }

  InitWindow(320, 240, "raylib");
  null0_host_load(wasmBytes, wasmBytesLen);
  
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
