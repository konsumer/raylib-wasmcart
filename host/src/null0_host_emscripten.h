#pragma once

#include <emscripten.h>
#include <string.h>
#include <stdio.h>

// called on cart init
EM_ASYNC_JS(void, null0_host_load, (unsigned char* wasmBytesPtr, int wasmBytesLen), {
    if (!Module.cart_wasi) {
        throw new Error("Set cart_wasi");
    }
    if (!Module.CartFS) {
        throw new Error("Set CartFS");
    }

    Module.cart_wasi.fs = new Module.CartFS(Module);
    const wasmBytes = Module.HEAPU8.slice(wasmBytesPtr, wasmBytesPtr+wasmBytesLen);
    const cartImports = {env: {}, wasi_snapshot_preview1: Module.cart_wasi};
    for (const n of Object.keys(Module)) {
        if (n.startsWith("_host")) {
            cartImports.env[n.replace(/^_host_/, "")] = Module[n];
        }
    }
    const { instance: { exports } } = await WebAssembly.instantiate(wasmBytes, cartImports);
    Module.cart = exports;
    Module.cart_wasi.start(exports);
    Module?.cart?.load && Module.cart.load();
    console.log(exports);
});

// called on cart update
EM_JS(void, null0_host_update, (double timeMs), {
    Module?.cart?.update && Module.cart.update(timeMs);
});

// called on cart unload
EM_JS(void, null0_host_unload, (), {
    Module?.cart?.unload && Module.cart.unload();
});

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
EM_JS(unsigned int, cart_set_pointer, (void* hostPtr, unsigned int len), {
});

// copy a string from cart to host
EM_JS(char*, cart_get_string, (unsigned int cartPtr), {
    const cartMem = new Uint8Array(Module.cart.memory.buffer.slice(cartPtr));
    const len = cartMem.findIndex(b => b === 0);
    if (len !== -1){
        return cart_get_pointer(cartPtr, len + 1);
    }
});

// copy a string from host to cart
EM_JS(unsigned int, cart_set_string, (char* hostPtr), {});

EMSCRIPTEN_KEEPALIVE void host_ClearBackground(unsigned int colorPtr) {
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    ClearBackground(*color);
    free(color);
}

EMSCRIPTEN_KEEPALIVE void host_DrawText(unsigned int textPtr, int posX, int posY, int fontSize, unsigned int colorPtr) {
    char* text = cart_get_string(textPtr);
    Color* color = cart_get_pointer(colorPtr, sizeof(Color));
    DrawText(text, posX, posY, fontSize, *color);
    free(text);
    free(color);
}

EMSCRIPTEN_KEEPALIVE void host_SetWindowSize(int width, int height) {
    TraceLog(LOG_INFO, "Resize: %dx%d", width, height);
    SetWindowSize(width, height);
}
