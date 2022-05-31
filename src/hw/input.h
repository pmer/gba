#pragma once

#include "mm.h"

// struct KeyInput {
//     u16 a:1;  // 0 = input, 1 = no input
//     u16 b:1;
//     u16 select:1;
//     u16 start:1;
//     u16 right:1;
//     u16 left:1;
//     u16 up:1;
//     u16 down:1;
//     u16 r:1;
//     u16 l:1;
// };
#define REG_KEYINPUT *(vu16*)(REG_BASE + 0x130)

#define KEY_IS_DOWN(key) (~REG_KEYINPUT & key)
#define KEY_IS_UP(key) (REG_KEYINPUT & key)

#define KEY_A 0x1
#define KEY_B 0x2
#define KEY_SELECT 0x4
#define KEY_START 0x8
#define KEY_RIGHT 0x10
#define KEY_LEFT 0x20
#define KEY_UP 0x40
#define KEY_DOWN 0x80
#define KEY_R 0x100
#define KEY_L 0x200

typedef enum eKeyIndex
{
	KI_A=0, KI_B, KI_SELECT, KI_START, 
	KI_RIGHT, KI_LEFT, KI_UP, KI_DOWN,
	KI_R, KI_L, KI_MAX
} eKeyIndex;
