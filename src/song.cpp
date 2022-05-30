#include "song.h"
#include "reg.h"

extern "C" {
#include <tonc_bios.h>
#include <tonc_core.h>
}

static void
notePlay(int note, int octave) noexcept {
    REG_SND1FREQ_ = SFREQ_RESET_ | SND_RATE(note, octave);
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
    REG_SNDSTAT_ = SSTAT_ENABLE_;
    REG_SNDDMGCNT_ = SDMG_BUILD_LR_(SDMG_SQR1, 7);
    REG_SNDDSCNT_ = SDS_DMG100_;

    REG_SND1SWEEP_ = SSW_OFF_;
    REG_SND1CNT_ = SSQR_ENV_BUILD_(12, 0, 7) | SSQR_DUTY1_2_;
    REG_SND1FREQ_ = 0;

    sos();
}
