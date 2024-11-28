#pragma once
#include <emscripten.h>
#include <string.h>
#include "raylib.h"
#include "cvector.h"

// these are arrays of complex-structures (that have pointer-fields)
static cvector_vector_type(Image) bucket_images = NULL;


// allocate cart-memory from host C
EM_JS(unsigned int, cart_malloc, (int size), {
  return Module.cart.malloc(size);
});

// free cart-memory from host C
EM_JS(void, cart_free, (unsigned int ptr), {
  Module.cart.free(ptr);
});

// copy a pointer from cart to host
EM_JS(void*, cart_get_pointer, (unsigned int cartPtr, unsigned int len), {
  const out = Module._malloc(len);
  const cartMem = new Uint8Array(Module.cart.memory.buffer).slice(cartPtr, cartPtr + len);
  Module.HEAPU8.set(cartMem, out);
  return out;
});

// copy a pointer from host to cart
EM_JS(unsigned int, cart_set_pointer, (void* hostPtr, unsigned int len, unsigned int cartPtr), {
  cartPtr = cartPtr || Module.cart.malloc(len);
  new Uint8Array(Module.cart.memory.buffer).set(Module.HEAPU8.slice(hostPtr, hostPtr + len), cartPtr);
  return cartPtr;
});

// copy a string from cart to host
EM_JS(char*, cart_get_string, (unsigned int cartPtr), {
  const cartMem = new Uint8Array(Module.cart.memory.buffer.slice(cartPtr));
  const len = cartMem.findIndex(b => b === 0);
  if (len !== -1) {
    return cart_get_pointer(cartPtr, len + 1);
  }
});

// copy a string from host to cart
EM_JS(unsigned int, cart_set_string, (char* hostPtr), {
  const hostMem = Module.HEAPU8.slice(hostPtr);
  const len = hostMem.findIndex(b => b === 0);
  if (len !== -1) {
    return cart_set_pointer(hostPtr, len + 1);
  }
});

EMSCRIPTEN_KEEPALIVE void host_SetWindowSize(unsigned int widthPtr, unsigned int heightPtr) {
    SetWindowSize(widthPtr, heightPtr);
}

EMSCRIPTEN_KEEPALIVE void host_ClearBackground(unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ClearBackground(*color);
    free((void*)color);
}

EMSCRIPTEN_KEEPALIVE unsigned int host_LoadFileText(unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    char* result = LoadFileText(fileName);
    return cart_set_string(result);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE void host_LoadImage(unsigned int outPtr, unsigned int fileNamePtr) {
    const char* fileName = cart_get_string(fileNamePtr);
    Image out = LoadImage(fileName);
    cart_set_pointer(&out, sizeof(out), outPtr);
    free((void*)fileName);
}

EMSCRIPTEN_KEEPALIVE void host_UnloadImage(unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    UnloadImage(*image);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_LoadTextureFromImage(unsigned int outPtr, unsigned int imagePtr) {
    Image* image = cart_get_pointer(imagePtr, sizeof(Image));
    Texture2D out = LoadTextureFromImage(*image);
    cart_set_pointer(&out, sizeof(out), outPtr);
    free((void*)image);
}

EMSCRIPTEN_KEEPALIVE void host_DrawTexture(unsigned int texturePtr, unsigned int posXPtr, unsigned int posYPtr, unsigned int tintPtr) {
    Texture2D* texture = cart_get_pointer(texturePtr, sizeof(Texture2D));
    Color* tint = cart_get_pointer(tintPtr, sizeof(Color));
    DrawTexture(*texture, posXPtr, posYPtr, *tint);
    free((void*)texture);
    free((void*)tint);
}

EMSCRIPTEN_KEEPALIVE void host_DrawText(unsigned int textPtr, unsigned int posXPtr, unsigned int posYPtr, unsigned int fontSizePtr, unsigned int colorPtr) {
    const char* text = cart_get_string(textPtr);
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawText(text, posXPtr, posYPtr, fontSizePtr, *color);
    free((void*)text);
    free((void*)color);
}




// called on cart init
EM_ASYNC_JS(void, null0_host_load, (unsigned char* wasmBytesPtr, int wasmBytesLen), {
  if (!Module.cart_wasi) {
    throw new Error("Set cart_wasi");
  }
  if (!Module.CartFS) {
    throw new Error("Set CartFS");
  }

  Module.cart_wasi.fs = new Module.CartFS(Module);
  const wasmBytes = Module.HEAPU8.slice(wasmBytesPtr, wasmBytesPtr + wasmBytesLen);
  const cartImports = {env : {}, wasi_snapshot_preview1 : Module.cart_wasi};
  for (const n of Object.keys(Module)) {
    if (n.startsWith("_host")) {
      cartImports.env[n.replace(/^_host_/, "")] = Module[n];
    }
  }
  const {instance : {exports}} = await WebAssembly.instantiate(wasmBytes, cartImports);
  Module.cart = exports;
  Module.cart_wasi.start(exports);
  Module?.cart?.load && Module.cart.load();
});

// called on cart update
EM_JS(void, null0_host_update, (double timeMs), {
  Module?.cart?.update && Module.cart.update(timeMs);
});

// called on cart unload
EM_JS(void, null0_host_unload, (), {
  Module?.cart?.unload && Module.cart.unload();
});
