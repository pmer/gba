#include "song.h"
#include "tonc.h"

#include "hw/audio.h"
#include "hw/int.h"

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

void
sing() noexcept {
    REG_SNDSTAT = SSTAT_ENABLE;
    REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1, 7);
    REG_SNDDSCNT = SDS_DMG100;

    REG_SND1SWEEP = SSW_OFF;
    REG_SND1CNT = SSQR_ENV_BUILD(12, 0, 7) | SSQR_DUTY1_2;
    REG_SND1FREQ = 0;

    sos();
}
