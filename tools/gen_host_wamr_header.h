#include <stdlib.h>
#include <string.h>

#include "wasm_export.h"

// Global reference to the WASM runtime instance
static wasm_module_inst_t module_inst;

void init_wamr_helpers(wasm_module_inst_t inst) { module_inst = inst; }

void* cart_get_pointer(unsigned int cartPtr, unsigned int len) {
  void* hostPtr = malloc(len);
  if (!hostPtr) return NULL;

  void* wasmPtr = wasm_runtime_addr_app_to_native(module_inst, cartPtr);
  if (!wasmPtr) {
    free(hostPtr);
    return NULL;
  }

  memcpy(hostPtr, wasmPtr, len);
  return hostPtr;
}

unsigned int cart_set_pointer(void* hostPtr, unsigned int len) {
  unsigned int cartPtr = wasm_runtime_malloc(module_inst, len);
  if (!cartPtr) return 0;

  void* wasmPtr = wasm_runtime_addr_app_to_native(module_inst, cartPtr);
  if (!wasmPtr) {
    wasm_runtime_free(module_inst, cartPtr);
    return 0;
  }

  memcpy(wasmPtr, hostPtr, len);
  return cartPtr;
}

char* cart_get_string(unsigned int cartPtr) {
  if (!cartPtr) return NULL;

  const char* wasmStr = wasm_runtime_addr_app_to_native(module_inst, cartPtr);
  if (!wasmStr) return NULL;

  size_t len = strlen(wasmStr) + 1;
  char* hostStr = malloc(len);
  if (!hostStr) return NULL;

  memcpy(hostStr, wasmStr, len);
  return hostStr;
}

unsigned int cart_set_string(char* hostStr) {
  if (!hostStr) return 0;

  size_t len = strlen(hostStr) + 1;
  unsigned int cartPtr = wasm_runtime_malloc(module_inst, len);
  if (!cartPtr) return 0;

  void* wasmPtr = wasm_runtime_addr_app_to_native(module_inst, cartPtr);
  if (!wasmPtr) {
    wasm_runtime_free(module_inst, cartPtr);
    return 0;
  }

  memcpy(wasmPtr, hostStr, len);
  return cartPtr;
}

unsigned int cart_malloc(int size) {
  return wasm_runtime_malloc(module_inst, size);
}

void cart_free(unsigned int ptr) { wasm_runtime_free(module_inst, ptr); }
