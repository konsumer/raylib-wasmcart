#pragma once
#include <stdlib.h>
#include <string.h>

#include "wasm_c_api.h"
#include "wasm_export.h"

// Global reference to the WASM runtime instance
static wasm_module_t module;
static wasm_module_inst_t module_inst;
static wasm_function_inst_t func;
static wasm_exec_env_t exec_env;
static wasm_function_inst_t cart_update = NULL;
static wasm_function_inst_t cart_unload = NULL;
static uint64_t update_args[1];

// copy a pointer from cart to host
void* cart_get_pointer(unsigned int cartPtr, unsigned int len) {
    void* out = malloc(len);
    memcpy(out, wasm_runtime_addr_app_to_native(module_inst, (uint64_t)cartPtr), len);
    return out;
}

// copy a pointer from host to cart
unsigned int cart_set_pointer(void* hostPtr, unsigned int len, unsigned int cartPtr) {
  if (cartPtr ==0) {
    cartPtr = wasm_runtime_module_malloc(module_inst, len, NULL);
  }
    if (cartPtr == 0) {
        return 0;
    }
    void* nativePtr = wasm_runtime_addr_app_to_native(module_inst, cartPtr);
    if (!nativePtr) {
        wasm_runtime_module_free(module_inst, cartPtr);
        return 0;
    }
    memcpy(nativePtr, hostPtr, len);
    return cartPtr;
}

// copy a string from cart to host
char* cart_get_string(unsigned int cartPtr) {
  void* nativePtr = wasm_runtime_addr_app_to_native(module_inst, cartPtr);
  unsigned int len = strlen(nativePtr) + 1;
  return cart_get_pointer(cartPtr, len);
}

// copy a string from host to cart
unsigned int cart_set_string(char* hostPtr) {
  unsigned int len = strlen(hostPtr) + 1;
  unsigned int cartPtr = wasm_runtime_module_malloc(module_inst, len, NULL);
  void* nativePtr = wasm_runtime_addr_app_to_native(module_inst, cartPtr);
  memcpy(nativePtr, hostPtr, len);
  return cartPtr;
}

// allocate cart-memory from host C
unsigned int cart_malloc(int size) {
  return wasm_runtime_module_malloc(module_inst, size, NULL);
}

// free cart-memory from host C
void cart_free (unsigned int ptr) {
  wasm_runtime_module_free(module_inst, ptr);
}
void host_SetWindowSize(wasm_exec_env_t exec_env, unsigned int widthPtr, unsigned int heightPtr) {
    SetWindowSize(widthPtr, heightPtr);
}

void host_ClearBackground(wasm_exec_env_t exec_env, unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ClearBackground(*color);
    free((void*)color);
}

unsigned int host_LoadFileText(wasm_exec_env_t exec_env, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    char* result = LoadFileText(fileName);
    return cart_set_string(result);
    free((void*)fileName);
}

void host_LoadImage(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Image out = LoadImage(fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
    free((void*)fileName);
}

void host_UnloadImage(wasm_exec_env_t exec_env, unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    UnloadImage(*image);
    free((void*)image);
}

void host_LoadTextureFromImage(wasm_exec_env_t exec_env, unsigned int outPtr, unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Texture2D out = LoadTextureFromImage(*image);
    cart_set_pointer(&out, sizeof(out), outPtr);
    free((void*)image);
}

void host_DrawTexture(wasm_exec_env_t exec_env, unsigned int texturePtr, unsigned int posXPtr, unsigned int posYPtr, unsigned int tintPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTexture(*texture, posXPtr, posYPtr, *tint);
    free((void*)texture);
    free((void*)tint);
}

void host_DrawText(wasm_exec_env_t exec_env, unsigned int textPtr, unsigned int posXPtr, unsigned int posYPtr, unsigned int fontSizePtr, unsigned int colorPtr) {
    const char* text = cart_get_string(textPtr);
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawText(text, posXPtr, posYPtr, fontSizePtr, *color);
    free((void*)text);
    free((void*)color);
}


static NativeSymbol native_symbols[] = {
  {"SetWindowSize", host_SetWindowSize, NULL},
  {"ClearBackground", host_ClearBackground, NULL},
  {"LoadFileText", host_LoadFileText, NULL},
  {"LoadImage", host_LoadImage, NULL},
  {"UnloadImage", host_UnloadImage, NULL},
  {"LoadTextureFromImage", host_LoadTextureFromImage, NULL},
  {"DrawTexture", host_DrawTexture, NULL},
  {"DrawText", host_DrawText, NULL}
};


// called on cart init
void null0_host_load(unsigned char* wasmBytes, int wasmBytesLen){
    uint32_t stack_size = 8092, heap_size = 8092;
    char error_buf[128];
    wasm_runtime_init();
    wasm_runtime_register_natives("env", native_symbols, sizeof(native_symbols) / sizeof(NativeSymbol));
    module = wasm_runtime_load(wasmBytes, wasmBytesLen, error_buf, sizeof(error_buf));
    module_inst = wasm_runtime_instantiate(module, stack_size, heap_size, error_buf, sizeof(error_buf));
    exec_env = wasm_runtime_create_exec_env(module_inst, stack_size);

    wasm_function_inst_t cart_load = wasm_runtime_lookup_function(module_inst, "load");
    cart_update = wasm_runtime_lookup_function(module_inst, "update");
    cart_unload = wasm_runtime_lookup_function(module_inst, "unload");

    wasm_application_execute_main(module_inst, 0, NULL);
    if (cart_load != NULL) {
        if (!wasm_runtime_call_wasm(exec_env, cart_load, 0, NULL)) {
            // not fatal, but this will help with troubleshooting
            printf("load: %s\n", wasm_runtime_get_exception(module_inst));
        }
    }
}

// called on cart update
void null0_host_update(double timeMs){
    if (cart_update != NULL) {
        update_args[0] = timeMs;
        wasm_runtime_call_wasm(exec_env, cart_update, 2, update_args);
    }
}

// called on cart unload
void null0_host_unload(){
    if (cart_unload != NULL) {
        if (!wasm_runtime_call_wasm(exec_env, cart_unload, 0, NULL)) {
            // not fatal, but this will help with troubleshooting
            printf("unload: %s\n", wasm_runtime_get_exception(module_inst));
        }
    }
}
