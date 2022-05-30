#pragma once

#include <tonc_types.h>

// I/O registers
#define MEM_IO_ 0x04000000
// Video RAM. Note: no 8bit write !!
#define MEM_VRAM_ 0x06000000

#define REG_BASE_ MEM_IO_

// Display control
#define REG_DISPCNT_ *(vu32*)(REG_BASE_+0x0)

#define DCNT_MODE0_ 0x1  // Sprite mode 0.
#define DCNT_MODE1_ 0x2  // Sprite mode 1.
#define DCNT_MODE2_ 0x3  // Sprite mode 2.
#define DCNT_MODE3_ 0x3  // Mode 3. Background 2 is a 240x160 @ 16bpp bitmap.
#define DCNT_MODE4_ 0x4  // Mode 4. 240x160 @ 8bpp bitmap.
#define DCNT_MODE5_ 0x5  // Mode 5. 160x128 @ 16bpp bitmap.
#define DCNT_BG1_ 0x100  // Enable background 1.
#define DCNT_BG2_ 0x200  // Enable background 2.
#define DCNT_BG3_ 0x400  // Enable background 3.
#define DCNT_BG4_ 0x800  // Enable background 4.

// Scanline count.
#define REG_VCOUNT_ *(vu16*)(REG_BASE_+0x6)

// Channel 1 Square wave with sweep.
#define REG_SND1SWEEP_ *(vu16*)(REG_BASE_+0x60)  // Sweep.
#define REG_SND1CNT_ *(vu16*)(REG_BASE_+0x62)	 // Control.
#define REG_SND1FREQ_ *(vu16*)(REG_BASE_+0x64)  // Frequency.

// Sweep
#define SSW_OFF_ 0x8  // Disable sweep.
// Control
#define SSQR_DUTY1_2_ 0x80  // 50% duty cycle: (####----)
#define SSQR_ENV_BUILD_(ivol, dir, time) (((ivol)<<12) | ((dir)<<11) | (((time)&7)<<8))
// Frequency
#define SFREQ_RESET_ 0x8000  // Reset sound.

// Sound status
#define REG_SNDSTAT_ *(vu16*)(REG_BASE_+0x84)
#define SSTAT_ENABLE_ 0x80  // Enable sound. Note: Enable before using any other sound registers.

// DMG channel control
#define REG_SNDDMGCNT_ *(vu16*)(REG_BASE_+0x80)
#define SDMG_BUILD_(lmode, rmode, lvol, rvol) (((rmode)<<12) | ((lmode)<<8) | (((rvol)&7)<<4) | ((lvol)&7))
#define SDMG_BUILD_LR_(mode, vol) SDMG_BUILD_(mode, mode, vol, vol)

// Direct Sound control
#define REG_SNDDSCNT_ *(vu16*)(REG_BASE_+0x82)
#define SDS_DMG100_ 0x2  // Tone generators at 100% volume.

// Video memory
#define vid_mem_ ((COLOR*)MEM_VRAM_)
#define m3_mem_ ((M3LINE*)MEM_VRAM_)
