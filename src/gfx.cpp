#include "gfx.h"
#include "math.h"

#include "hw/video.h"

void
drawRect(i32 left, i32 top, i32 width, i32 height, Color color) noexcept {
    i32 right = left + width;
    i32 bottom = top + height;
    for (i32 y = top; y < bottom; y++) {
        for (i32 x = left; x < right; x++) {
            M3_MEM[y][x] = color;
        }
    }
}

void
drawLine(i32 x, i32 y, i32 x2, i32 y2, Color color) noexcept {
    i32 w = x2 - x;
    i32 h = y2 - y;
    i32 dx1 = 0, dx2 = 0, dy1 = 0, dy2 = 0;

    if (w < 0) dx1 = dx2 = -1;
    else if (w > 0) dx1 = dx2 = 1;
    if (h < 0) dy1 = -1;
    else if (h > 0) dy1 = 1;

    i32 longest = abs(w);
    i32 shortest = abs(h);
    if (shortest > longest) {
        shortest ^= longest;
        longest ^= shortest;
        shortest ^= longest;

        if (h < 0) dy2 = -1;
        else if (h > 0) dy2 = 1;
        dx2 = 0;
    }

    i32 numerator = longest >> 1;
    for (i32 i = 0; i <= longest; i++) {
        M3_MEM[y][x] = color;
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
    while (REG_VCOUNT >= SCREEN_HEIGHT);
    while (REG_VCOUNT < SCREEN_HEIGHT);
}
