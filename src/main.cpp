#include "gfx.h"
#include "pong.h"
#include "print.h"
#include "song.h"
#include "tonc.h"

#include "hw/video.h"

static void
drawPattern() noexcept {
    REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

    int t = 0;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        int x, y;
        for (y = 0; y < SCREEN_HEIGHT; ++y) {
            for (x = 0; x < SCREEN_WIDTH; ++x) {
                int base = x & y;
                M3_MEM[y][x] = COLOR_SAFE(base + t * 5, base + t * 3, base + t);
            }
        }
        ++t;
    }
#pragma clang diagnostic pop
}

static void
halt() noexcept {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        VBlankIntrWait();
    }
#pragma clang diagnostic pop
}

int
main() noexcept {
    //printf_("Hello, world!");

    pong();

    //REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

    /*
    mode3();

    COLOR salmon = RGB15(31, 5, 12);
    drawRect(20, 20, SCREEN_WIDTH - 40, SCREEN_HEIGHT - 40, salmon);

    COLOR green = RGB15(1, 24, 16);
    drawLine(20 - 1, 20 - 1, 0 - 1, 0 - 1, green); // Top-left.
    drawLine(220, 20 - 1, 240, 0 - 1, green); // Top-right.
    drawLine(20 - 1, 140, 0 - 1, 160, green); // Bottom-left.
    drawLine(220, 140, 240, 160, green); // Bottom-right.
    */

    /*
    irq_init(nullptr);
    irq_add(II_VBLANK, nullptr);

    //sing();

    drawPattern();

    halt();
    */
}
