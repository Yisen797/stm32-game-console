/**
 * KY-023 双轴摇杆驱动 (F103C8T6)
 *
 *   VRx = PA0  (ADC12_IN0)
 *   VRy = PA1  (ADC12_IN1)
 *   SW  = PA2  (按键, 按下为低)
 */

#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include "stm32f1xx_hal.h"

/* ---- ADC 句柄 (外部引用) ---- */
extern ADC_HandleTypeDef hadc1;

/* ---- API ---- */
void joystick_init(void);
void joystick_read(uint16_t *x, uint16_t *y);
uint8_t joystick_pressed(void);   /* 返回 1=按下 */

#endif
