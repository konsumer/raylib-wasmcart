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