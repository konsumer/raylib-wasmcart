#pragma once

#include <emscripten.h>
#include <string.h>
#include <stdio.h>

// just get the size of a file at a  url
EM_ASYNC_JS(int, file_size_real, (char* url), {
    return fetch(UTF8ToString(url)).then(response => Number(response.headers.get("content-length")));
});

// perform a fetch of a url
EM_ASYNC_JS(void, file_read_real, (unsigned char* output, char* url), {
    const bytes = await fetch(UTF8ToString(url)).then(r => r.arrayBuffer() );
    Module.HEAPU8.set(new Uint8Array(bytes), output);
});

// called to setup cart
EM_ASYNC_JS(void, null0_host_load, (unsigned char* wasmBytesPtr, int wasmBytesLen), {
    if (!Module.cart_wasi) {
        throw new Error("Set cart_wasi");
    }
    if (!Module.CartFS) {
        throw new Error("Set CartFS");
    }
    Module.cart_wasi.fs = new Module.CartFS(Module);
    const wasmBytes = Module.HEAPU8.slice(wasmBytesPtr, wasmBytesPtr+wasmBytesLen);
    const cartImports = {null0: {}, wasi_snapshot_preview1: Module.cart_wasi};
    for (const n of Object.keys(Module)) {
        if (n.startsWith("_host")) {
            cartImports.null0[n.replace(/^_host_/, "")] = Module[n];
        }
    }
    const { instance: {exports} } = await WebAssembly.instantiate(wasmBytes, cartImports);
    Module.cart = exports;

    if (exports._start) {
        exports._start();
    }
    if (exports.load) {
        exports.load();
    }
});

// called when host is updated
EM_JS(void, null0_host_update, (double time), {
    if (Module?.cart?.update) {
        Module.cart.update(time);
    }
});

// called when host is unloaded
EM_JS(void, null0_host_unload, (), {
    if (Module?.cart?.unload) {
        Module.cart.unload();
    }
});

// copy a pointer from cart into host
EM_JS(void*, null0_host_get_cart_pointer, (unsigned int cart_ptr, unsigned int size), {
    const out = Module._malloc(size);
    Module.HEAPU8.set(Module.cart.memory.buffer.slice(cart_ptr, cart_ptr+size), out);
    return out;
});

// copy string from cart  into host
EM_JS(char*, null0_host_get_cart_string, (unsigned int cart_ptr), {
    let len = 0;
    while (len < 1024*1024) {
        if (Module.cart.memory.buffer[cart_ptr + len] === 0) {
            break;
        }
        len++;
    }
    return null0_host_get_cart_pointer(cart_ptr, len);
});

EMSCRIPTEN_KEEPALIVE void host_clear(unsigned int colorPtr) {
    Color* color = null0_host_get_cart_pointer(colorPtr, sizeof(Color));
    // printf("clear(%u, %u, %u, %u)\n", color->r, color->g, color->b, color->a);
    ClearBackground(*color);
    free(color);
}

EMSCRIPTEN_KEEPALIVE void host_draw_text(unsigned int textPtr, int posX, int posY, int fontSize, unsigned int colorPtr) {
    Color* color = null0_host_get_cart_pointer(colorPtr, sizeof(Color));
    char* text = null0_host_get_cart_string(textPtr);
    // printf("draw_text(%s, %dx%d, %d, %u, %u, %u, %u)\n", text, posX, posY, fontSize, color->r, color->g, color->b, color->a);
    DrawText(text, posX, posY, fontSize, *color);
    free(color);
}

EMSCRIPTEN_KEEPALIVE void host_trace(unsigned int textPtr) {
    printf("%s\n", null0_host_get_cart_string(textPtr));
}

EMSCRIPTEN_KEEPALIVE int host_measure_text(unsigned int textPtr, int fontSize) {
    char* text = null0_host_get_cart_string(textPtr);
    // printf("measure_text(%s, %d)\n", text, fontSize);
    return MeasureText(text, fontSize);
}
