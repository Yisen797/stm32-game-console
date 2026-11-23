/**
 * ATK-MD0280 触摸屏驱动 (适配 F103C8T6)
 *
 *   PEN (触摸中断) = PA3
 */

#ifndef __ATK_MD0280_TOUCH_H
#define __ATK_MD0280_TOUCH_H

#include "atk_md0280.h"

/* ---- 触摸中断引脚 ---- */
#define ATK_MD0280_TOUCH_PEN_PORT             GPIOA
#define ATK_MD0280_TOUCH_PEN_PIN              GPIO_PIN_3

#define ATK_MD0280_TOUCH_READ_PEN()           HAL_GPIO_ReadPin(ATK_MD0280_TOUCH_PEN_PORT, ATK_MD0280_TOUCH_PEN_PIN)

/* ---- 兼容原驱动旧宏名 ---- */
#define ATK_MD0280_TOUCH_PEN_GPIO_PORT             ATK_MD0280_TOUCH_PEN_PORT
#define ATK_MD0280_TOUCH_PEN_GPIO_PIN              ATK_MD0280_TOUCH_PEN_PIN
#define ATK_MD0280_TOUCH_PEN_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

/* ---- 参数 ---- */
#define ATK_MD0280_TOUCH_READ_TIMES           5
#define ATK_MD0280_TOUCH_READ_DISCARD         1
#define ATK_MD0280_TOUCH_READ_RANGE           50

/* ---- 返回值 ---- */
#define ATK_MD0280_TOUCH_EOK                  0
#define ATK_MD0280_TOUCH_ERROR                1
#define ATK_MD0280_TOUCH_EMPTY                2

/* ---- 函数声明 ---- */
void atk_md0280_touch_init(void);
uint8_t atk_md0280_touch_scan(uint16_t *x, uint16_t *y);

#endif
