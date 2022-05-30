#pragma once

#include <tonc_types.h>

// I/O registers
#define MEM_IO_ 0x04000000
// Video RAM. Note: no 8bit write !!
#define MEM_VRAM_ 0x06000000

#define REG_BASE_ MEM_IO_

// Display control
#define REG_DISPCNT_ *(vu32*)(REG_BASE_+0x0000)
#define DCNT_MODE3_ 0x0003  // Mode 3; bg2: 240x160@16 bitmap
#define DCNT_BG2_ 0x0400  // Enable bg 2

// Scanline count
#define REG_VCOUNT_ *(vu16*)(REG_BASE_+0x0006)

// Channel 1 Square wave with sweep
#define REG_SND1SWEEP_ *(vu16*)(REG_BASE_+0x0060)  // Sweep
#define REG_SND1CNT_ *(vu16*)(REG_BASE_+0x0062)	 // Control
#define REG_SND1FREQ_ *(vu16*)(REG_BASE_+0x0064)  // Frequency

// Sweep
#define SSW_OFF_ 0x0008  // Disable sweep
// Control
#define SSQR_DUTY1_2_ 0x0080	 // 50% duty cycle (####----)
#define SSQR_ENV_BUILD_(ivol, dir, time) (((ivol)<<12) | ((dir)<<11) | (((time)&7)<<8))
// Frequency
#define SFREQ_RESET_ 0x8000  // Reset sound

// Sound status
#define REG_SNDSTAT_ *(vu16*)(REG_BASE_+0x0084)
#define SSTAT_ENABLE_ 0x0080  // Enable sound. NOTE: enable before using any other sound regs

// DMG channel control
#define REG_SNDDMGCNT_ *(vu16*)(REG_BASE_+0x0080)
#define SDMG_BUILD_(lmode, rmode, lvol, rvol) (((rmode)<<12) | ((lmode)<<8) | (((rvol)&7)<<4) | ((lvol)&7))
#define SDMG_BUILD_LR_(mode, vol) SDMG_BUILD_(mode, mode, vol, vol)

// Direct Sound control
#define REG_SNDDSCNT_ *(vu16*)(REG_BASE_+0x0082)
#define SDS_DMG100_ 0x0002  // Tone generators at 100% volume

// Video memory
#define vid_mem_ ((COLOR*)MEM_VRAM_)
#define m3_mem_ ((M3LINE*)MEM_VRAM_)
