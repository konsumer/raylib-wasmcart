#pragma once
#include <stdio.h>

// called on cart init
void null0_host_load(unsigned char* wasmBytes, int wasmBytesLen){}

// called on cart update
void null0_host_update(double timeMs){}

// called on cart unload
void null0_host_unload(){}


// read a real file from fs
unsigned char* file_read_real(char* filename, unsigned int* fileSizePtr) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        TraceLog(LOG_ERROR, "Cannot open file %s", filename);
        return NULL;
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);  // or fseek(fp, 0, SEEK_SET); // Reset to beginning of file

    *fileSizePtr = file_size;
    unsigned char* output = malloc(file_size);
    if (output == NULL) {
        TraceLog(LOG_ERROR, "Memory allocation failed");
        fclose(fp);
        return NULL;
    }

    // Read the file
    size_t bytes_read = fread(output, 1, file_size, fp);
    fclose(fp);

    if (bytes_read != file_size) {
        TraceLog(LOG_ERROR, "Failed to read entire file");
        free(output);
        return NULL;
    }

    return output;
}
