#pragma once
#include <stdio.h>

#include "wasm_c_api.h"
#include "wasm_export.h"

static wasm_module_t module;
static wasm_module_inst_t module_inst;
static wasm_function_inst_t func;
static wasm_exec_env_t exec_env;

// copy a pointer from cart to host
void* cart_get_pointer(unsigned int cartPtr, unsigned int len) {
    void* out = malloc(len);
    memcpy(out, wasm_runtime_addr_app_to_native(module_inst, (uint64_t)cartPtr), len);
    return out;
}

void host_InitWindow(wasm_exec_env_t exec_env, int width, int height, char* title) {
    InitWindow(width, height, title);
}

void host_ClearBackground(wasm_exec_env_t exec_env, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ClearBackground(*color);
    free(color);
}

void host_DrawText(wasm_exec_env_t exec_env, char* text, int posX, int posY, int fontSize, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawText(text, posX, posY, fontSize, *color);
    free(color);
}

void host_SetWindowSize(wasm_exec_env_t exec_env, int width, int height) {
    TraceLog(LOG_INFO, "Resize: %dx%d", width, height);
    SetWindowSize(width, height);
}

static NativeSymbol native_symbols[] = {
    {"InitWindow", host_InitWindow, "(ii$)"},
    {"ClearBackground", host_ClearBackground, "(i)"},
    {"DrawText", host_DrawText, "($iiii)"},
    {"SetWindowSize", host_SetWindowSize, "(ii)"},
};

// called on cart init
void null0_host_load(unsigned char* wasmBytes, int wasmBytesLen){
    uint32_t stack_size = 8092, heap_size = 8092;
    char error_buf[128];
    wasm_runtime_init();
    wasm_runtime_register_natives("env", native_symbols, sizeof(native_symbols) / sizeof(NativeSymbol));
    module = wasm_runtime_load(wasmBytes, wasmBytesLen, error_buf, sizeof(error_buf));
    module_inst = wasm_runtime_instantiate(module, stack_size, heap_size, error_buf, sizeof(error_buf));
}

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
