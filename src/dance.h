/**
 * dance.h — 蔡徐坤运球舞 (几何图形版, 6帧)
 */
#ifndef __DANCE_H
#define __DANCE_H

#include "stm32f1xx_hal.h"

#define DANCE_FRAMES 6

/* 每帧定义: 身体倾斜角, 左臂角, 右臂角, 左腿角, 右腿角, 球偏移(x,y)
   角度: 0=上, 90=右, 180=下, 270=左 */
typedef struct {
    int16_t body_a;    /* 身体角 */
    int16_t la_a;      /* 左臂角 */
    int16_t ra_a;      /* 右臂角 */
    int16_t ll_a;      /* 左腿角 */
    int16_t rl_a;      /* 右腿角 */
    int16_t ball_dx;   /* 球X偏移 */
    int16_t ball_dy;   /* 球Y偏移 */
} DanceFrame;

static const DanceFrame dance_frames[DANCE_FRAMES] = {
    {270, 220, 300, 260, 280,  5,-15},  /* 持球 */
    {265, 210, 310, 260, 280, 15,-10},  /* 左摆 */
    {275, 230, 290, 250, 300, 10,-20},  /* 踢腿 */
    {275, 250, 270, 260, 280, 15,-10},  /* 右摆 */
    {265, 210, 310, 280, 250,  5,-20},  /* 左踢 */
    {268, 200, 320, 260, 280, 20, 10},  /* 运球 */
};

void dance_draw_frame(uint16_t cx, uint16_t cy, int frame);

#endif
