#pragma once

#include "hw/int.h"

extern "C" {

// tonc_bios.h
void VBlankIntrDelay(u32 count);
void VBlankIntrWait(void);

// tonc_core.h
extern const unsigned int __snd_rates[12];
#define SND_RATE(note, oct) (2048 - (__snd_rates[note] >> (4 + (oct))))

}
