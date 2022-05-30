#pragma once

#include <tonc_types.h>

#define SCREEN_WIDTH_ 240
#define SCREEN_HEIGHT_ 160

#define CLR_BLACK_ 0x0000
#define CLR_RED_ 0x001F
#define CLR_BLUE_ 0x7C00
#define CLR_WHITE_ 0x7FFF

#define RED_SHIFT 0
#define GREEN_SHIFT 5
#define BLUE_SHIFT 10

static inline COLOR RGB15_SAFE_(int red, int green, int blue) noexcept {
    return ((red & 31) << RED_SHIFT) + ((green & 31) << GREEN_SHIFT) + ((blue & 31) << BLUE_SHIFT);
}

static inline COLOR RGB15_(u8 red, u8 green, u8 blue) noexcept {
    return (red << RED_SHIFT) + (green << GREEN_SHIFT) + (blue << BLUE_SHIFT);
}
