#include "print.h"
#include "song.h"
#include "tonc.h"

#include "hw/audio.h"
#include "hw/int.h"

const u32 __snd_rates[12]=
{
	8013, 7566, 7144, 6742, // C , C#, D , D#
	6362, 6005, 5666, 5346, // E , F , F#, G
	5048, 4766, 4499, 4246  // G#, A , A#, B
};
#define SND_RATE(note, oct) (2048 - (__snd_rates[note] >> (4 + (oct))))


static void
notePlay(int note, int octave) noexcept {
    printf_("notePlay(%d, %d) which is frequency %d", note, octave, SND_RATE(note, octave));
    REG_SND1FREQ = SFREQ_RESET | SND_RATE(note, octave);

}

static void
sos() noexcept {
    const int NOTES = 21;
    const u8 lens[NOTES] = {
        1, 1, 4, 1, 1, 4, 
        4, 1, 1, 1, 1, 1, 4, 
        2, 2, 2, 2, 2, 1, 1, 2 // TODO: Fix rythm
        };
    const u8 notes[NOTES] = {
        0x02, 0x05, 0x12, 0x02, 0x05, 0x12, 
        0x14, 0x15, 0x14, 0x15, 0x14, 0x10, 0x09,
        0x09, 0x02, 0x05, 0x09, 0x02, 0x05, 0x07, 0x04
        };

    for (int i = 0; i < NOTES; i++) {
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
