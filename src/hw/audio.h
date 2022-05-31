#pragma once

#include "int.h"
#include "reg.h"

// Channel 1 Square wave with sweep.
#define REG_SND1SWEEP *(vu16*)(REG_BASE + 0x60)  // Sweep.
#define REG_SND1CNT *(vu16*)(REG_BASE + 0x62)  // Control.
#define REG_SND1FREQ *(vu16*)(REG_BASE + 0x64)  // Frequency.

// Sweep
#define SSW_OFF 0x8  // Disable sweep.
// Control
#define SSQR_DUTY1_2 0x80  // 50% duty cycle: (####----)
#define SSQR_ENV_BUILD(ivol, dir, time) (((ivol) << 12) | ((dir) << 11) | (((time) & 7) << 8))
// Frequency
#define SFREQ_RESET 0x8000  // Reset sound.

// Sound status
#define REG_SNDSTAT *(vu16*)(REG_BASE + 0x84)
#define SSTAT_ENABLE 0x80  // Enable sound. Note: Enable before using any other sound registers.

// DMG channel control
#define REG_SNDDMGCNT *(vu16*)(REG_BASE + 0x80)
#define SDMG_SQR1 0x1
#define SDMG_BUILD(lmode, rmode, lvol, rvol) (((rmode) << 12) | ((lmode) << 8) | (((rvol) & 7) << 4) | ((lvol) & 7))
#define SDMG_BUILD_LR(mode, vol) SDMG_BUILD(mode, mode, vol, vol)

// Direct Sound control
#define REG_SNDDSCNT *(vu16*)(REG_BASE + 0x82)
#define SDS_DMG100 0x2  // Tone generators at 100% volume.

