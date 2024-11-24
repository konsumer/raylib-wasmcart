#include "raylib.h"
#include "physfs.h"
#include <stdlib.h>
#include "cvector.h"

// Read a file in physfs memory
// physfs must be mounted first
unsigned char* read_physfs_file(char* fileName, PHYSFS_sint64* fileLength) {
    PHYSFS_file* file = PHYSFS_openRead(fileName);
    if (file == NULL) {
        TraceLog(LOG_ERROR, "Failed to open file: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        return NULL;
    }
    *fileLength = PHYSFS_fileLength(file);
    unsigned char* buffer = (unsigned char*)malloc(*fileLength);
    if (buffer == NULL) {
        PHYSFS_close(file);
        TraceLog(LOG_ERROR, "Failed to allocate mem for cart.");
        return NULL;
    }

    PHYSFS_sint64 bytesRead = PHYSFS_readBytes(file, buffer, *fileLength);
    if (bytesRead != *fileLength) {
        TraceLog(LOG_ERROR, "Failed to read file: %s\n",  PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        free(buffer);
        PHYSFS_close(file);
        return NULL;
    }
    PHYSFS_close(file);
    return buffer;
}

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
