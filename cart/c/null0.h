// This is the API for null0 carts

#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULL0_EXPORT(n) __attribute__((export_name(n)))
#define NULL0_IMPORT(n) __attribute__((import_module("null0"), import_name(n)))

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

// implement these in your cart
NULL0_EXPORT("load")
void load();

NULL0_EXPORT("update")
void update(f64 time);

NULL0_EXPORT("unload")
void unload();

// these let host manage memory

NULL0_EXPORT("malloc")
void* _null0_malloc(size_t size) {
  return malloc(size);
}

NULL0_EXPORT("free")
void _null0_free(void* ptr) {
  free(ptr);
}

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} Color;

typedef struct {
    f32 x;                // Vector x component
    f32 y;                // Vector y component
} Vector2;

#define WIDTH 320
#define HEIGHT 240

#define LIGHTGRAY  (Color){ 200, 200, 200, 255 }   // Light Gray
#define GRAY       (Color){ 130, 130, 130, 255 }   // Gray
#define DARKGRAY   (Color){ 80, 80, 80, 255 }      // Dark Gray
#define YELLOW     (Color){ 253, 249, 0, 255 }     // Yellow
#define GOLD       (Color){ 255, 203, 0, 255 }     // Gold
#define ORANGE     (Color){ 255, 161, 0, 255 }     // Orange
#define PINK       (Color){ 255, 109, 194, 255 }   // Pink
#define RED        (Color){ 230, 41, 55, 255 }     // Red
#define MAROON     (Color){ 190, 33, 55, 255 }     // Maroon
#define GREEN      (Color){ 0, 228, 48, 255 }      // Green
#define LIME       (Color){ 0, 158, 47, 255 }      // Lime
#define DARKGREEN  (Color){ 0, 117, 44, 255 }      // Dark Green
#define SKYBLUE    (Color){ 102, 191, 255, 255 }   // Sky Blue
#define BLUE       (Color){ 0, 121, 241, 255 }     // Blue
#define DARKBLUE   (Color){ 0, 82, 172, 255 }      // Dark Blue
#define PURPLE     (Color){ 200, 122, 255, 255 }   // Purple
#define VIOLET     (Color){ 135, 60, 190, 255 }    // Violet
#define DARKPURPLE (Color){ 112, 31, 126, 255 }    // Dark Purple
#define BEIGE      (Color){ 211, 176, 131, 255 }   // Beige
#define BROWN      (Color){ 127, 106, 79, 255 }    // Brown
#define DARKBROWN  (Color){ 76, 63, 47, 255 }      // Dark Brown
#define WHITE      (Color){ 255, 255, 255, 255 }   // White
#define BLACK      (Color){ 0, 0, 0, 255 }         // Black
#define BLANK      (Color){ 0, 0, 0, 0 }           // Blank (Transparent)
#define MAGENTA    (Color){ 255, 0, 255, 255 }     // Magenta
#define RAYWHITE   (Color){ 245, 245, 245, 255 }   // My own White (raylib logo)

// max-size for trace messages
#ifndef NULL0_TRACE_SIZE
#define NULL0_TRACE_SIZE 1024 * 1024 * 1024
#endif

NULL0_IMPORT("trace")
void _null0_trace_real(char* str);

char null0_traceBuffer[NULL0_TRACE_SIZE];

// Log a string (using printf-style)
void trace(const char* format, ...) {
  va_list args;
  va_start(args, format);
  vsnprintf(null0_traceBuffer, NULL0_TRACE_SIZE, format, args);
  va_end(args);
  _null0_trace_real(null0_traceBuffer);
}

NULL0_IMPORT("clear") void clear(Color color);
NULL0_IMPORT("draw_text") void draw_text(char* text, i32 posX, i32 posY, i32 fontSize, Color color);
NULL0_IMPORT("measure_text") Vector2 measure_text(char* text, unsigned int font, int fontSize);
