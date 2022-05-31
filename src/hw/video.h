#pragma once

#include "int.h"
#include "mm.h"

// Display control
// struct DisplayController {
//     // Selects the BG mode.
//     u16 bgMode:3;  // Mode 0 to mode 2 = tiled, mode 3 to mode 5 = bitmap
//     // Read-only value that is always 0 for Game Boy Advance games.
//     u16 isGameBoyColorMode:1;
//     // Used in modes 4 and 5, which have two frame buffers.
//     u16 displayFrameSelection:1;  // 0 = frame buffer 0, 1 = frame buffer 1
//     // When accessing OAM or OBJ VRAM during an H-Blank, this bit needs to be set. However, it slows things down.
//     u16 hBlankIntervalOBJProcessingFlag:1;  // 0 = Enable, 1 = Disable
//     // Whether OBJ characters are memory mapped as 2D or 1D.
//     u16 objCharacterVRAMMappingFormat:1;  // 0 = 2D, 1 = 1D
//     // Stop the image processing circuit and display white on the screen.
//     u16 forcedBlank:1;  // 0 = disabled, 1 = enabled
//     // Whether BG0-3 and OBJ are displayed.
//     u16 individualScreensDisplay:5;  // For each bit: 0 = off, 1 = on
//     // Whether windows 0 and 1 are displayed;
//     u16 windows0DisplayFlag:1;
//     u16 windows1DisplayFlag:1;
//     // Whether the OBJ window is displayed.
//     u16 objWindowDisplayFlag:1;
// };
// struct DisplayControl {
//     u16 mode:3;
//     u16 gb:1;
//     u16 pageSelect:1;
//     u16 allowOamInHBlank:1;
//     u16 objectMapping:1;
//     u16 forceScreenBlank:1;
//     u16 enableBg0Layer:1;
//     u16 enableBg1Layer:1;
//     u16 enableBg2Layer:1;
//     u16 enableBg3Layer:1;
//     u16 enableObjLayer:1;
//     u16 enableWindow0:1;
//     u16 enableWindow1:1;
//     u16 enableWindowObj:1;
// };
#define REG_DISPCNT *(vu16*)(REG_BASE + 0x0)

#define DCNT_MODE0 0x1  // Sprite mode 0.
#define DCNT_MODE1 0x2  // Sprite mode 1.
#define DCNT_MODE2 0x3  // Sprite mode 2.
#define DCNT_MODE3 0x3  // Mode 3. Background 2 is a 240x160 @ 16bpp bitmap.
#define DCNT_MODE4 0x4  // Mode 4. 240x160 @ 8bpp bitmap.
#define DCNT_MODE5 0x5  // Mode 5. 160x128 @ 16bpp bitmap.
#define DCNT_BG0 0x100  // Enable background 0.
#define DCNT_BG1 0x200  // Enable background 1.
#define DCNT_BG2 0x400  // Enable background 2.
#define DCNT_BG3 0x800  // Enable background 3.
#define DCNT_OBJ 0x1000

// struct DisplayStatus {
//     u16 vblankStatus:1;  // 1 = in a V-Blank
//     u16 hblankStatus:1;  // 1 = in an H-blank
//     u16 vcountTriggerStatus:1;  // 1 = current scanline == vcountTriggerValue
//     u16 vblankInterruptRequest:1;  // 1 = fire an interrupt on V-Blank
//     u16 hblankInterruptRequest:1;  // 1 = fire an interrupt on H-Blank
//     u16 vcountInterruptRequest:1;  // 1 = fire an interrupt on scanline == vcountTriggerValue
//     u16 unused:2;
//     u16 vcountTriggerValue:8;
// };
#define REG_DISPSTAT *(vu16*)(REG_BASE + 0x4)

#define DSTAT_IN_VBL 0x1
#define DSTAT_IN_HBL 0x2
#define DSTAT_IN_VCT 0x4
#define DSTAT_VBL_IRQ 0x8
#define DSTAT_HBL_IRQ 0x10
#define DSTAT_VCT_IRQ 0x20
#define DSTAT_VCT_MASK 0xff00
#define DSTAT_VCT_SHIFT 8
#define DSTAT_VCT(x) (x) << 8

// Scanline count.
// struct VCounter {
//     // Which of the 228 LCD lines is being rendered.
//     // Values 160 to 227 mean a vertical blanking period.
//     u16 vCounter:8;
//     u16 unused:8;
// };
#define REG_VCOUNT *(vu16*)(REG_BASE + 0x6)

// Video memory
#define M3_MEM ((M3Line*)MEM_VRAM)
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

#define CLR_BLACK 0x0
#define CLR_RED COLOR(31, 0, 0)
#define CLR_LIME COLOR(0, 31, 0)
#define CLR_YELLOW COLOR(0, 31, 31)
#define CLR_BLUE COLOR(0, 0, 31)
#define CLR_MAG COLOR(31, 0, 31)
#define CLR_CYAN COLOR(31, 31, 0)
#define CLR_WHITE COLOR(31, 31, 31)

#define RED_SHIFT 0
#define GREEN_SHIFT 5
#define BLUE_SHIFT 10

// struct Color {
//     u16 red:5;
//     u16 green:5;
//     u16 blue:5;
//     u16 unused:1;
// };
typedef u16 Color;

typedef Color M3Line[SCREEN_WIDTH];

static inline
Color COLOR_SAFE(u16 red, u16 green, u16 blue) noexcept {
    return ((red & 31) << RED_SHIFT) | ((green & 31) << GREEN_SHIFT) | ((blue & 31) << BLUE_SHIFT);
}

static inline
Color COLOR(u16 red, u16 green, u16 blue) noexcept {
    return (red << RED_SHIFT) | (green << GREEN_SHIFT) | (blue << BLUE_SHIFT);
}
