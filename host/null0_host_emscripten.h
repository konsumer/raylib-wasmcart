#pragma once

#include <emscripten.h>
#include "raylib.h"

// called when host is loaded
EM_JS(void, null0_host_load, (int argc, char** argv), {
});

// called when host is updated
EM_JS(void, null0_host_update, (double time), {
});

// called when host is unloaded
EM_JS(void, null0_host_unload, (), {
});

// copy a pointer from cart
EM_JS(void*, null0_host_get_cart_pointer, (unsigned int cart_ptr, void* host_ptr, unsigned int size), {
  return 0;
});

EMSCRIPTEN_KEEPALIVE void host_clear(unsigned int colorPtr) {
  Color* color = malloc(sizeof(Color));
  null0_host_get_cart_pointer(colorPtr, color, sizeof(Color));
  if (color != NULL){
    ClearBackground(*color);
    free(color);
  }
}


EMSCRIPTEN_KEEPALIVE void host_draw_text(char *text, int posX, int posY, int fontSize, unsigned int colorPtr) {
  Color* color = malloc(sizeof(Color));
  null0_host_get_cart_pointer(colorPtr, color, sizeof(Color));
  if (color != NULL){
    DrawText(text, posX, posY, fontSize, *color);
    free(color);
  }
}