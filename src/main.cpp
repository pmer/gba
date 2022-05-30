#include "gfx.h"
#include "pong.h"
#include "print.h"
#include "reg.h"
#include "song.h"
#include "video.h"

extern "C" {
#include <tonc_bios.h>
}

static void
drawPattern() noexcept {
    mode3();

    int t = 0;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        int x, y;
        for (y = 0; y < SCREEN_HEIGHT_; ++y) {
            for (x = 0; x < SCREEN_WIDTH_; ++x) {
                int base = x & y;
                vid_mem_[x + y * SCREEN_WIDTH_] = RGB15_SAFE_(base + t * 5, base + t * 3, base + t);
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

    COLOR salmon = RGB15_(31, 5, 12);
    drawRect(20, 20, SCREEN_WIDTH - 40, SCREEN_HEIGHT - 40, salmon);

    COLOR green = RGB15_(1, 24, 16);
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
