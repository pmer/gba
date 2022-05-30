#include "print.h"

#include <tonc.h>

static void
notePlay(int note, int octave) noexcept {
    REG_SND1FREQ = SFREQ_RESET | SND_RATE(note, octave);
}

static void
sos() noexcept {
    const u8 lens[6] = {1, 1, 4, 1, 1, 4};
    const u8 notes[6] = {2, 5, 18, 2, 5, 18};

    for (int i = 0; i < 6; i++) {
        notePlay(notes[i] & 15, notes[i] >> 4);
        VBlankIntrDelay(8 * lens[i]);
    }
}

static void
sing() noexcept {
    REG_SNDSTAT = SSTAT_ENABLE;
    REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1, 7);
    REG_SNDDSCNT = SDS_DMG100;

    REG_SND1SWEEP = SSW_OFF;
    REG_SND1CNT = SSQR_ENV_BUILD(12, 0, 7) | SSQR_DUTY1_2;
    REG_SND1FREQ = 0;

    sos();
}

static void
drawRect(s32 left, s32 top, s32 width, s32 height, COLOR color) noexcept {
    s32 right = left + width;
    s32 bottom = top + height;
    for (s32 y = top; y < bottom; y++) {
        for (s32 x = left; x < right; x++) {
            m3_mem[y][x] = color;
        }
    }
}

static s32
abs(s32 i) noexcept {
    s32 mask = i >> 31;
    return (i ^ mask) - mask;
}

static void
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
        m3_mem[y][x] = color;
        numerator += shortest;
        if (numerator > longest) {
            numerator -= longest;
            x += dx1;
            y += dy1;
        }
        else {
            x += dx2;
            y += dy2;
        }
    }
}

static void
mode3() noexcept {
    // Set GBA rendering context to mode 3 with bitmap rendering.
    REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;
}

static void
drawPattern() noexcept {
    mode3();

    int t = 0;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        int x, y;
        for (y = 0; y < SCREEN_HEIGHT; ++y) {
            for (x = 0; x < SCREEN_WIDTH; ++x) {
                int base = x & y;
                vid_mem[x + y * SCREEN_WIDTH] = RGB15_SAFE(base + t * 5, base + t * 3, base + t);
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
    printf_("Hello, world!");

    //REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

    mode3();

    COLOR salmon = RGB15(31, 5, 12);
    drawRect(20, 20, SCREEN_WIDTH - 40, SCREEN_HEIGHT - 40, salmon);

    COLOR green = RGB15(1, 24, 16);
    drawLine(20-1, 20-1, 0-1, 0-1, green); // Top-left.
    drawLine(220, 20-1, 240, 0-1, green); // Top-right.
    drawLine(20-1, 140, 0-1, 160, green); // Bottom-left.
    drawLine(220, 140, 240, 160, green); // Bottom-right.

    irq_init(nullptr);
    irq_add(II_VBLANK, nullptr);

    sing();

    drawPattern();

    halt();
}
