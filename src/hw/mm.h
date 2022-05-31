#pragma once

// System ROM, execute-only, len=16kb, bus=32bit
#define MEM_BIOS 0x00000000
// External working RAM, len=256kb, bus=16bit
#define MEM_EW 0x02000000
// Internal working RAM, len=32kb, bus=32bit
#define MEM_IW 0x03000000
// I/O and registers, len=1kb, bus=16bit
#define MEM_IO 0x04000000
#define REG_BASE MEM_IO
// Background palette and sprite palette, len=1kb, bus=16bit, no 8bit I/O
#define MEM_PAL 0x05000000
// Background and sprite data, len=96kb, bus=16bit, no 8bit I/O
#define MEM_VRAM 0x06000000
// Sprite controls, len=1kb, bus=32bit, no 8bit I/O
#define MEM_OAM 0x07000000
// Game Pak ROM in the "wait state 0" block, len=32mb, bus=16bit
#define MEM_PACK_ROM 0x08000000
// Game Pak RAM, len=64kb, bus=8bit
#define MEM_PACK_RAM 0x0e000000
