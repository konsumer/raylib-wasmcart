#include "null0.h"
#include "stdio.h"

f32 w = 0;

const u32 fontSize = 20;

int main() {
  // width for centered text
  Vector2 m = measure_text("Hello from null0", 0, fontSize);
  w = (WIDTH / 2) - (m.x / 2);
  trace("Hello from hello. %fx%f", m.x, m.y);
  return 0;
}

void update(f64 time) {
  clear(BLACK);
  draw_text("Hello from null0", w + 1, (HEIGHT / 2) + 1, fontSize, BLUE);
  draw_text("Hello from null0", w, HEIGHT / 2, fontSize, RED);
}
