#include <stdio.h>
#include <stdlib.h>
#include <physfs.h>
#include <raylib.h>
#include <string.h>

unsigned char *RaylibPhysFSLoadFileDataCallback(const char *fileName, int *dataSize) {
    TraceLog(LOG_INFO, "FS: LOAD Data %s", fileName);

    PHYSFS_file *file = PHYSFS_openRead(fileName);
    if (!file) {
        TraceLog(LOG_ERROR, "FS: Failed to open file for reading: %s", fileName);
        *dataSize = 0;
        return NULL;
    }

    // Get file size
    PHYSFS_sint64 fileLength = PHYSFS_fileLength(file);
    if (fileLength < 0) {
        TraceLog(LOG_ERROR, "FS: Failed to get file size: %s", fileName);
        PHYSFS_close(file);
        *dataSize = 0;
        return NULL;
    }

    // Allocate memory for file data
    unsigned char *data = (unsigned char *)MemAlloc((size_t)fileLength);
    if (!data) {
        TraceLog(LOG_ERROR, "FS: Failed to allocate memory for file: %s", fileName);
        PHYSFS_close(file);
        *dataSize = 0;
        return NULL;
    }

    // Read file data
    PHYSFS_sint64 bytesRead = PHYSFS_readBytes(file, data, fileLength);
    PHYSFS_close(file);

    if (bytesRead != fileLength) {
        TraceLog(LOG_ERROR, "FS: Failed to read file: %s", fileName);
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
        TraceLog(LOG_ERROR, "FS: Failed to open file for writing: %s", fileName);
        return false;
    }

    PHYSFS_sint64 bytesWritten = PHYSFS_writeBytes(file, data, dataSize);
    PHYSFS_close(file);

    if (bytesWritten != dataSize) {
        TraceLog(LOG_ERROR, "FS: Failed to write file: %s", fileName);
        return false;
    }

    return true;
}

char *RaylibPhysFSLoadFileTextCallback(const char *fileName) {
    PHYSFS_file *file = PHYSFS_openRead(fileName);
    TraceLog(LOG_INFO, "FS: LOAD Text %s", fileName);

    if (!file) {
        TraceLog(LOG_ERROR, "FS: Failed to open text file for reading: %s", fileName);
        return NULL;
    }

    // Get file size
    PHYSFS_sint64 fileLength = PHYSFS_fileLength(file);
    if (fileLength < 0) {
        TraceLog(LOG_ERROR, "FS: Failed to get text file size: %s", fileName);
        PHYSFS_close(file);
        return NULL;
    }

    // Allocate memory for text (including null terminator)
    char *text = (char *)MemAlloc((size_t)fileLength + 1);
    if (!text) {
        TraceLog(LOG_ERROR, "FS: Failed to allocate memory for text file: %s", fileName);
        PHYSFS_close(file);
        return NULL;
    }

    // Read file data
    PHYSFS_sint64 bytesRead = PHYSFS_readBytes(file, text, fileLength);
    PHYSFS_close(file);

    if (bytesRead != fileLength) {
        TraceLog(LOG_ERROR, "FS: Failed to read text file: %s", fileName);
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
        TraceLog(LOG_ERROR, "FS: Failed to open text file for writing: %s", fileName);
        return false;
    }

    size_t length = strlen(text);
    PHYSFS_sint64 bytesWritten = PHYSFS_writeBytes(file, text, length);
    PHYSFS_close(file);

    if (bytesWritten != length) {
        TraceLog(LOG_ERROR, "FS: Failed to write text file: %s", fileName);
        return false;
    }

    return true;
}

// these are similar to raylib file functions

bool FileExistsFS(const char *fileName) {
    PHYSFS_Stat stat;
    if (!PHYSFS_stat(fileName, &stat)) return false;
    return stat.filetype == PHYSFS_FILETYPE_REGULAR;
}

bool DirectoryExistsFS(const char *dirPath) {
    PHYSFS_Stat stat;
    if (!PHYSFS_stat(dirPath, &stat)) return false;
    return stat.filetype == PHYSFS_FILETYPE_DIRECTORY;
}

int GetFileLengthFS(const char *fileName) {
    PHYSFS_file *file = PHYSFS_openRead(fileName);
    if (!file) return 0;
    
    PHYSFS_sint64 length = PHYSFS_fileLength(file);
    PHYSFS_close(file);
    return (int)length;
}

int MakeDirectoryFS(const char *dirPath) {
    return PHYSFS_mkdir(dirPath) ? 0 : -1;
}

bool ChangeDirectoryFS(const char *dir) {
    // PhysFS doesn't have a concept of "current directory"
    // but we can check if the directory exists
    return DirectoryExistsFS(dir);
}

bool IsPathFileFS(const char *path) {
    return FileExistsFS(path);
}

FilePathList LoadDirectoryFilesFS(const char *dirPath) {
    FilePathList list = { 0 };
    char **files = PHYSFS_enumerateFiles(dirPath);
    char **i;
    
    // First count the number of files
    for (i = files; *i != NULL; i++) {
        list.count++;
    }
    
    if (list.count == 0) {
        PHYSFS_freeList(files);
        return list;
    }
    
    // Allocate memory for paths
    list.paths = (char **)MemAlloc(list.count * sizeof(char *));
    
    // Copy paths
    int index = 0;
    for (i = files; *i != NULL; i++) {
        char fullPath[512];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, *i);
        list.paths[index] = (char *)MemAlloc(strlen(fullPath) + 1);
        strcpy(list.paths[index], fullPath);
        index++;
    }
    
    PHYSFS_freeList(files);
    return list;
}

FilePathList LoadDirectoryFilesExFS(const char *basePath, const char *filter, bool scanSubdirs) {
    FilePathList list = { 0 };
    char **files = PHYSFS_enumerateFiles(basePath);
    char **i;
    
    // First pass: count matching files
    for (i = files; *i != NULL; i++) {
        char fullPath[512];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", basePath, *i);
        
        PHYSFS_Stat stat;
        if (!PHYSFS_stat(fullPath, &stat)) continue;
        
        if (stat.filetype == PHYSFS_FILETYPE_DIRECTORY && scanSubdirs) {
            FilePathList subList = LoadDirectoryFilesExFS(fullPath, filter, true);
            list.count += subList.count;
            UnloadDirectoryFiles(subList); // Free temporary list
        } else if (stat.filetype == PHYSFS_FILETYPE_REGULAR) {
            // Check if file matches filter
            if (filter && *filter) {
                const char *ext = strrchr(*i, '.');
                if (ext && strcasecmp(ext + 1, filter) == 0) {
                    list.count++;
                }
            } else {
                list.count++;
            }
        }
    }
    
    if (list.count == 0) {
        PHYSFS_freeList(files);
        return list;
    }
    
    // Allocate memory for paths
    list.paths = (char **)MemAlloc(list.count * sizeof(char *));
    int pathIndex = 0;
    
    // Second pass: copy matching files
    for (i = files; *i != NULL; i++) {
        char fullPath[512];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", basePath, *i);
        
        PHYSFS_Stat stat;
        if (!PHYSFS_stat(fullPath, &stat)) continue;
        
        if (stat.filetype == PHYSFS_FILETYPE_DIRECTORY && scanSubdirs) {
            FilePathList subList = LoadDirectoryFilesExFS(fullPath, filter, true);
            for (int j = 0; j < subList.count; j++) {
                list.paths[pathIndex] = (char *)MemAlloc(strlen(subList.paths[j]) + 1);
                strcpy(list.paths[pathIndex], subList.paths[j]);
                pathIndex++;
            }
            UnloadDirectoryFiles(subList);
        } else if (stat.filetype == PHYSFS_FILETYPE_REGULAR) {
            if (filter && *filter) {
                const char *ext = strrchr(*i, '.');
                if (ext && strcasecmp(ext + 1, filter) == 0) {
                    list.paths[pathIndex] = (char *)MemAlloc(strlen(fullPath) + 1);
                    strcpy(list.paths[pathIndex], fullPath);
                    pathIndex++;
                }
            } else {
                list.paths[pathIndex] = (char *)MemAlloc(strlen(fullPath) + 1);
                strcpy(list.paths[pathIndex], fullPath);
                pathIndex++;
            }
        }
    }
    
    PHYSFS_freeList(files);
    return list;
}

long GetFileModTimeFS(const char *fileName) {
    PHYSFS_Stat stat;
    if (!PHYSFS_stat(fileName, &stat)) return 0;
    return (long)stat.modtime;
}

bool InitFS(char* filename){
    PHYSFS_ErrorCode error;

    if (filename == NULL) {
        if (!PHYSFS_init("/")) {
            error = PHYSFS_getLastErrorCode();
            char* errorMsg = PHYSFS_getErrorByCode(error);
            TraceLog(LOG_ERROR, "FS: Init %d: %s\n", error, errorMsg);
            return false;
        }
    } else {
        if (!PHYSFS_init(filename)) {
            error = PHYSFS_getLastErrorCode();
            char* errorMsg = PHYSFS_getErrorByCode(error);
            TraceLog(LOG_ERROR, "FS: Init %d: %s\n", error, errorMsg);
            return false;
        }
        if (!PHYSFS_mount(filename, "/", 0)) {
            error = PHYSFS_getLastErrorCode();
            char* errorMsg = PHYSFS_getErrorByCode(error);
            TraceLog(LOG_ERROR, "FS: Mount %d: %s\n", error, errorMsg);
            return false;
        }
    }

    SetLoadFileDataCallback(RaylibPhysFSLoadFileDataCallback);
    SetSaveFileDataCallback(RaylibPhysFSSaveFileDataCallback);
    SetLoadFileTextCallback(RaylibPhysFSLoadFileTextCallback);
    SetSaveFileTextCallback(RaylibPhysFSSaveFileTextCallback);

    TraceLog(LOG_INFO, "FS: Initialized filesystem (%s)", filename);
    return true;
}

void CloseFS() {
    PHYSFS_deinit();
}