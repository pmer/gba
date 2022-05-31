#pragma once

// Execute-only, len=16kb, bus=32bit
#define MEM_BIOS 0x00000000
// External work RAM, len=256kb, bus=16bit
#define MEM_EW 0x02000000
// Len=32kb, bus=32bit
#define MEM_IW 0x03000000
// I/O "registers", len=1kb, bus=16bit
#define MEM_IO 0x04000000
// Background palette and sprite palette, len=1kb, bus=16bit
#define MEM_PAL 0x05000000
// Background and sprite data, len=96kb, bus=16bit (no 8-bit writes?)
#define MEM_VRAM 0x06000000
// Sprite controls, len=1kb, bus=32bit
#define MEM_OAM 0x07000000
// Game Pak ROM, len=variable, bus=16bit
#define MEM_PACK 0x08000000
// Cart RAM, len=variable, bus=8bit
#define MEM_CART 0x0e000000

#define REG_BASE MEM_IO
