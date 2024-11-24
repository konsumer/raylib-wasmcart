#include "null0.h"
#include "stdio.h"

f32 w = 0;

int main() {
  // width for centered text
  int tw = measure_text("Hello from null0", 20);
  w = (WIDTH / 2) - (tw / 2);
  trace("Hello from hello. %dx%d", tw, 10);
  return 0;
}

void update(f64 time) {
  clear(BLACK);
  draw_text("Hello from null0", w + 1, (HEIGHT / 2) + 1, 20, BLUE);
  draw_text("Hello from null0", w, HEIGHT / 2, 20, RED);
}
