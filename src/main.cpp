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
drawPattern() noexcept {
    // Set GBA rendering context to mode 3 with bitmap rendering.
    REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

    int t = 0;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        int x, y;
        for (y = 0; y < 160; ++y) {
            for (x = 0; x < 240; ++x) {
                vid_mem[x + y * 240] = ((((x & y) + t) & 0x1F) << 10) |
                                       ((((x & y) + t * 3) & 0x1F) << 5) |
                                       ((((x & y) + t * 5) & 0x1F) << 0);
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
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

    irq_init(0);
    irq_add(II_VBLANK, 0);

    REG_SNDSTAT = SSTAT_ENABLE;
    REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1, 7);
    REG_SNDDSCNT = SDS_DMG100;

    REG_SND1SWEEP = SSW_OFF;
    REG_SND1CNT = SSQR_ENV_BUILD(12, 0, 7) | SSQR_DUTY1_2;
    REG_SND1FREQ = 0;

    sos();

    drawPattern();

    halt();
}
