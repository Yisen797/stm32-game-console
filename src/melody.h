/**
 * melody.h — Brainiac Maniac 简谱旋律
 */
#ifndef __MELODY_H
#define __MELODY_H

#include "stm32f1xx_hal.h"

#define MELODY_LEN 42

typedef struct { uint16_t freq; uint16_t ms; } Note;
extern const Note melody[MELODY_LEN];

#endif
