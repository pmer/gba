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
#define DCNT_BG1 0x100  // Enable background 1.
#define DCNT_BG2 0x200  // Enable background 2.
#define DCNT_BG3 0x400  // Enable background 3.
#define DCNT_BG4 0x800  // Enable background 4.

// Scanline count.
#define REG_VCOUNT *(vu16*)(REG_BASE + 0x6)

// Video memory
#define VID_MEM ((Color*)MEM_VRAM)
#define M3_MEM ((M3Line*)MEM_VRAM)
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

#define CLR_BLACK Color{}
#define CLR_RED Color{31, 0, 0}
#define CLR_BLUE Color{0, 0, 31}
#define CLR_WHITE Color{31, 31, 31}

#define RED_SHIFT 0
#define GREEN_SHIFT 5
#define BLUE_SHIFT 10

struct Color {
    u16 red:5;
    u16 green:5;
    u16 blue:5;
    u16 unused:1;
};

typedef Color M3Line[SCREEN_WIDTH];

static inline
Color colorSafe(u16 red, u16 green, u16 blue) noexcept {
    return Color{static_cast<u16>(red & 31), static_cast<u16>(green & 31), static_cast<u16>(blue & 31)};
}

static inline
Color color(u16 red, u16 green, u16 blue) noexcept {
    return Color{red, green, blue};
}
