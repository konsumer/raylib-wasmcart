#include <stdio.h>
#include <stdlib.h>
#include <physfs.h>
#include <raylib.h>
#include <string.h>

unsigned char *RaylibPhysFSLoadFileDataCallback(const char *fileName, int *dataSize) {
    PHYSFS_file *file = PHYSFS_openRead(fileName);
    if (!file) {
        TraceLog(LOG_ERROR, "PHYSFS: Failed to open file for reading: %s", fileName);
        *dataSize = 0;
        return NULL;
    }

    // Get file size
    PHYSFS_sint64 fileLength = PHYSFS_fileLength(file);
    if (fileLength < 0) {
        TraceLog(LOG_ERROR, "PHYSFS: Failed to get file size: %s", fileName);
        PHYSFS_close(file);
        *dataSize = 0;
        return NULL;
    }

    // Allocate memory for file data
    unsigned char *data = (unsigned char *)MemAlloc((size_t)fileLength);
    if (!data) {
        TraceLog(LOG_ERROR, "PHYSFS: Failed to allocate memory for file: %s", fileName);
        PHYSFS_close(file);
        *dataSize = 0;
        return NULL;
    }

    // Read file data
    PHYSFS_sint64 bytesRead = PHYSFS_readBytes(file, data, fileLength);
    PHYSFS_close(file);

    if (bytesRead != fileLength) {
        TraceLog(LOG_ERROR, "PHYSFS: Failed to read file: %s", fileName);
        MemFree(data);
        *dataSize = 0;
        return NULL;
    }

    *dataSize = (int)fileLength;
    return data;
}

bool RaylibPhysFSSaveFileDataCallback(const char *fileName, void *data, int dataSize) {
    PHYSFS_file *file = PHYSFS_openWrite(fileName);
    if (!file) {
        TraceLog(LOG_ERROR, "PHYSFS: Failed to open file for writing: %s", fileName);
        return false;
    }

    PHYSFS_sint64 bytesWritten = PHYSFS_writeBytes(file, data, dataSize);
    PHYSFS_close(file);

    if (bytesWritten != dataSize) {
        TraceLog(LOG_ERROR, "PHYSFS: Failed to write file: %s", fileName);
        return false;
    }

    return true;
}

char *RaylibPhysFSLoadFileTextCallback(const char *fileName) {
    PHYSFS_file *file = PHYSFS_openRead(fileName);
    if (!file) {
        TraceLog(LOG_ERROR, "PHYSFS: Failed to open text file for reading: %s", fileName);
        return NULL;
    }

    // Get file size
    PHYSFS_sint64 fileLength = PHYSFS_fileLength(file);
    if (fileLength < 0) {
        TraceLog(LOG_ERROR, "PHYSFS: Failed to get text file size: %s", fileName);
        PHYSFS_close(file);
        return NULL;
    }

    // Allocate memory for text (including null terminator)
    char *text = (char *)MemAlloc((size_t)fileLength + 1);
    if (!text) {
        TraceLog(LOG_ERROR, "PHYSFS: Failed to allocate memory for text file: %s", fileName);
        PHYSFS_close(file);
        return NULL;
    }

    // Read file data
    PHYSFS_sint64 bytesRead = PHYSFS_readBytes(file, text, fileLength);
    PHYSFS_close(file);

    if (bytesRead != fileLength) {
        TraceLog(LOG_ERROR, "PHYSFS: Failed to read text file: %s", fileName);
        MemFree(text);
        return NULL;
    }

    // Null terminate the string
    text[fileLength] = '\0';
    return text;
}

bool RaylibPhysFSSaveFileTextCallback(const char *fileName, char *text) {
    PHYSFS_file *file = PHYSFS_openWrite(fileName);
    if (!file) {
        TraceLog(LOG_ERROR, "PHYSFS: Failed to open text file for writing: %s", fileName);
        return false;
    }

    size_t length = strlen(text);
    PHYSFS_sint64 bytesWritten = PHYSFS_writeBytes(file, text, length);
    PHYSFS_close(file);

    if (bytesWritten != length) {
        TraceLog(LOG_ERROR, "PHYSFS: Failed to write text file: %s", fileName);
        return false;
    }

    return true;
}

bool InitFS(char* filename){
    PHYSFS_ErrorCode error;

    if (filename == NULL) {
        if (!PHYSFS_init("/")) {
            error = PHYSFS_getLastErrorCode();
            char* errorMsg = PHYSFS_getErrorByCode(error);
            TraceLog(LOG_ERROR, "PhysFS - Init %d: %s\n", error, errorMsg);
            return false;
        }
    } else {
        if (!PHYSFS_init(filename)) {
            error = PHYSFS_getLastErrorCode();
            char* errorMsg = PHYSFS_getErrorByCode(error);
            TraceLog(LOG_ERROR, "PhysFS - Init %d: %s\n", error, errorMsg);
            return false;
        }
        if (!PHYSFS_mount(filename, "/", 0)) {
            error = PHYSFS_getLastErrorCode();
            char* errorMsg = PHYSFS_getErrorByCode(error);
            TraceLog(LOG_ERROR, "PhysFS - Mount %d: %s\n", error, errorMsg);
            return false;
        }
    }

    SetLoadFileDataCallback(RaylibPhysFSLoadFileDataCallback);
    SetSaveFileDataCallback(RaylibPhysFSSaveFileDataCallback);
    SetLoadFileTextCallback(RaylibPhysFSLoadFileTextCallback);
    SetSaveFileTextCallback(RaylibPhysFSSaveFileTextCallback);

    TraceLog(LOG_INFO, "Initialized filesystem (%s)", filename);
    return true;
}

void CloseFS() {
    PHYSFS_deinit();
}