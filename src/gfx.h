#pragma once

#include "hw/int.h"
#include "hw/video.h"

/**
 * Draw rectangle from left, top (inclusive) with width, height (exclusive).
 */
void
drawRect(i32 left, i32 top, i32 width, i32 height, Color color) noexcept;

/**
 * Draw line from x, y (inclusive) to x2, y2 (inclusive).
 */
void
drawLine(i32 x, i32 y, i32 x2, i32 y2, Color color) noexcept;

u16*
flip() noexcept;

void
vsync() noexcept;
