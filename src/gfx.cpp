#include "gfx.h"
#include "math.h"
#include "reg.h"
#include "video.h"

extern "C" {
#include <tonc_types.h>
}

void
mode3() noexcept {
    REG_DISPCNT_ = DCNT_MODE3_ | DCNT_BG2_;
}

#include <tonc.h>
void
drawRect(s32 left, s32 top, s32 width, s32 height, COLOR color) noexcept {
    s32 right = left + width;
    s32 bottom = top + height;
    for (s32 y = top; y < bottom; y++) {
        for (s32 x = left; x < right; x++) {
            m3_mem_[y][x] = color;
        }
    }
}

void
drawLine(s32 x, s32 y, s32 x2, s32 y2, COLOR color) noexcept {
    s32 w = x2 - x;
    s32 h = y2 - y;
    s32 dx1 = 0, dx2 = 0, dy1 = 0, dy2 = 0;

    if (w < 0) dx1 = dx2 = -1;
    else if (w > 0) dx1 = dx2 = 1;
    if (h < 0) dy1 = -1;
    else if (h > 0) dy1 = 1;

    s32 longest = abs(w);
    s32 shortest = abs(h);
    if (shortest > longest) {
        shortest ^= longest;
        longest ^= shortest;
        shortest ^= longest;

        if (h < 0) dy2 = -1;
        else if (h > 0) dy2 = 1;
        dx2 = 0;
    }

    s32 numerator = longest >> 1;
    for (s32 i = 0; i <= longest; i++) {
        m3_mem_[y][x] = color;
        numerator += shortest;
        if (numerator > longest) {
            numerator -= longest;
            x += dx1;
            y += dy1;
        } else {
            x += dx2;
            y += dy2;
        }
    }
}

void
vsync() noexcept {
    while (REG_VCOUNT_ >= SCREEN_HEIGHT_);
    while (REG_VCOUNT_ < SCREEN_HEIGHT_);
}
