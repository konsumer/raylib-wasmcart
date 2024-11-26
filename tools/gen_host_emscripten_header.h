#include <emscripten.h>
#include <stdio.h>
#include <string.h>

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
