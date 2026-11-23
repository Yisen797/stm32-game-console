/**
 * gfx.h — 游戏绘图工具
 */
#ifndef __GFX_H
#define __GFX_H

#include "stm32f1xx_hal.h"
#include "atk_md0280.h"

#define FW 10   /* 吃豆人半径 */

uint32_t gfx_sqrt(uint32_t n);
void gfx_circle(int16_t cx, int16_t cy, uint8_t r, uint16_t color);
void gfx_pacman(int16_t cx, int16_t cy, int mouth, uint16_t color);

#endif
