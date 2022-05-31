#pragma once

#include "int.h"
#include "reg.h"

// Display control
#define REG_DISPCNT *(vu32*)(REG_BASE + 0x0)

#define DCNT_MODE0 0x1  // Sprite mode 0.
#define DCNT_MODE1 0x2  // Sprite mode 1.
#define DCNT_MODE2 0x3  // Sprite mode 2.
#define DCNT_MODE3 0x3  // Mode 3. Background 2 is a 240x160 @ 16bpp bitmap.
#define DCNT_MODE4 0x4  // Mode 4. 240x160 @ 8bpp bitmap.
#define DCNT_MODE5 0x5  // Mode 5. 160x128 @ 16bpp bitmap.
#define DCNT_BG0 0x100  // Enable background 0.
#define DCNT_BG1 0x200  // Enable background 1.
#define DCNT_BG2 0x400  // Enable background 2.
#define DCNT_BG3 0x800  // Enable background 3.
#define DCNT_OBJ 0x1000

// Scanline count.
#define REG_VCOUNT *(vu16*)(REG_BASE + 0x6)

// Video memory
#define VID_MEM ((Color*)MEM_VRAM)
#define M3_MEM ((M3Line*)MEM_VRAM)
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

#define CLR_BLACK 0x0
#define CLR_RED COLOR(31, 0, 0)
#define CLR_LIME COLOR(0, 31, 0)
#define CLR_YELLOW COLOR(0, 31, 31)
#define CLR_BLUE COLOR(0, 0, 31)
#define CLR_MAG COLOR(31, 0, 31)
#define CLR_CYAN COLOR(31, 31, 0)
#define CLR_WHITE COLOR(31, 31, 31)

#define RED_SHIFT 0
#define GREEN_SHIFT 5
#define BLUE_SHIFT 10

// struct Color {
//     u16 red:5;
//     u16 green:5;
//     u16 blue:5;
//     u16 unused:1;
// };
typedef u16 Color;

typedef Color M3Line[SCREEN_WIDTH];

static inline
Color COLOR_SAFE(u16 red, u16 green, u16 blue) noexcept {
    return ((red & 31) << RED_SHIFT) | ((green & 31) << GREEN_SHIFT) | ((blue & 31) << BLUE_SHIFT);
}

static inline
Color COLOR(u16 red, u16 green, u16 blue) noexcept {
    return (red << RED_SHIFT) | (green << GREEN_SHIFT) | (blue << BLUE_SHIFT);
}
