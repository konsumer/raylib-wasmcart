#include "nill0_host_macros.hs"

HOST_FUNCTION(Texture2D, LoadTextureFromImage, (Image, image)) {
    return LoadTextureFromImage(image);
}

HOST_FUNCTION(void, DrawTexture, (Texture2D, texture), (int, posX), (int, posY), (Color, tint)) {
  DrawTexture(texture, posX, posY, tint);
}

#ifdef EMSCRIPTEN

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

#else // WAMR

// called on cart init
void null0_host_load(unsigned char* wasmBytesPtr, int wasmBytesLen) {}

// called on cart update
void null0_host_update(double timeMs) {}

// called on cart unload
void null0_host_unload() {}

#endif // WAMR