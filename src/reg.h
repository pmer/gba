#pragma once

#include <tonc_types.h>

#define MEM_IO_ 0x04000000  // I/O registers
#define MEM_VRAM_ 0x06000000  // Video RAM. Note: no 8bit write !!

#define REG_BASE_ MEM_IO_

#define REG_DISPCNT_ *(vu32*)(REG_BASE_+0x0000)	// Display control
#define DCNT_MODE3_ 0x0003	// Mode 3; bg2: 240x160@16 bitmap
#define DCNT_BG2_ 0x0400	// Enable bg 2

#define REG_VCOUNT_ *(vu16*)(REG_BASE_+0x0006)	// Scanline count

#define vid_mem_ ((COLOR*)MEM_VRAM_)
#define m3_mem_ ((M3LINE*)MEM_VRAM_)
