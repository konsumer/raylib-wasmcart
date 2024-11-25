#include <stddef.h>

// Platform-specific definitions
#ifdef EMSCRIPTEN
    #include <emscripten.h>
    #define HOST_DECL EMSCRIPTEN_KEEPALIVE
    #define HOST_ENV_PARAM
    #define HOST_ENV_ARG
#else
    #include "wasm_export.h"
    #define HOST_DECL static
    #define HOST_ENV_PARAM wasm_exec_env_t exec_env,
    #define HOST_ENV_ARG exec_env,
#endif

// Parameter handling macros
#define PARAM_PTR(type, name) unsigned int name##Ptr
#define CONVERT_PTR(type, name) \
    type* name##_ptr = cart_get_pointer(name##Ptr, sizeof(type)); \
    type name = *name##_ptr; \
    free(name##_ptr);

// Return value handling macros
#define RETURN_VALUE(type, value) \
    type* result_ptr = malloc(sizeof(type)); \
    *result_ptr = value; \
    unsigned int cart_ptr = cart_set_pointer(result_ptr, sizeof(type)); \
    free(result_ptr); \
    return cart_ptr;

// Main function macro
#define HOST_FUNCTION(return_type, name, ...) \
    HOST_DECL unsigned int name(HOST_ENV_PARAM MAP_PARAMS(__VA_ARGS__)) { \
        MAP_CONVERSIONS(__VA_ARGS__) \
        return_type result = _##name(MAP_ARGS(__VA_ARGS__)); \
        RETURN_VALUE(return_type, result) \
    } \
    static return_type _##name(__VA_ARGS__)

// Void function macro
#define HOST_FUNCTION_VOID(name, ...) \
    HOST_DECL void name(HOST_ENV_PARAM MAP_PARAMS(__VA_ARGS__)) { \
        MAP_CONVERSIONS(__VA_ARGS__) \
        _##name(MAP_ARGS(__VA_ARGS__)); \
    } \
    static void _##name(__VA_ARGS__)

// Parameter mapping macros (same as before)
#define MAP_PARAMS(...) CONCAT(MAP_PARAMS_, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define MAP_CONVERSIONS(...) CONCAT(MAP_CONVERSIONS_, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define MAP_ARGS(...) CONCAT(MAP_ARGS_, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

#ifdef EMSCRIPTEN

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
    const cartPtr = Module.cart.malloc(len);
    new Uint8Array(Module.cart.memory.buffer).set(Module.HEAPU8.slice(hostPtr, hostPtr+len), cartPtr);
    return cartPtr;
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
EM_JS(unsigned int, cart_set_string, (char* hostPtr), {
    const hostMem = Module.HEAPU8.slice(hostPtr);
    const len = hostMem.findIndex(b => b === 0);
    if (len !== -1){
        return cart_set_pointer(hostPtr, len + 1);
    }
});

#else

// MEMORY

// copy a pointer from cart to host
void* cart_get_pointer(unsigned int cartPtr, unsigned int len);

// copy a pointer from host to cart
unsigned int cart_set_pointer(void* hostPtr, unsigned int len);

// copy a string from cart to host
char* cart_get_string(unsigned int cartPtr);

// copy a string from host to cart
unsigned int cart_set_string(char* hostPtr);

// allocate cart-memory from host C
unsigned int cart_malloc(int size);

// free cart-memory from host C
void cart_free (unsigned int ptr);

#endif

// Example usage:
/*
HOST_FUNCTION(Texture2D, LoadTextureFromImage, (Image, image)) {
    return LoadTextureFromImage(image);
}
*/

