/**
 * gfx.c — 绘图工具实现
 */
#include "gfx.h"

uint32_t gfx_sqrt(uint32_t n) {
    uint32_t x=n, y=(x+1)/2;
    while(y<x){x=y;y=(x+n/x)/2;}
    return x;
}

static int in_bounds(int16_t x, int16_t y){
    return x>=0&&x<320&&y>=0&&y<320;
}
void gfx_circle(int16_t cx, int16_t cy, uint8_t r, uint16_t c)
{
    for(int dy=-r;dy<=r;dy++){
        int hw=(int)(gfx_sqrt((int32_t)r*r-dy*dy)+0.5f);
        for(int dx=-hw;dx<=hw;dx++){
            int16_t px=cx+dx, py=cy+dy;
            if(in_bounds(px,py)) atk_md0280_draw_point(px,py,c);
        }
    }
}

void gfx_pacman(int16_t cx, int16_t cy, int mouth, uint16_t color)
{
    int mh=(mouth==0)?1:7;
    gfx_circle(cx,cy,FW,color);
    for(int dy=0;dy<=FW;dy++){
        int dx_max=mh*dy/FW;
        for(int dx=-dx_max;dx<=dx_max;dx++){
            int16_t px=cx+dx, py=cy+dy;
            if(in_bounds(px,py)) atk_md0280_draw_point(px,py,ATK_MD0280_BLACK);
        }
    }
    if(in_bounds(cx+3,cy-4)) atk_md0280_draw_point(cx+3,cy-4,ATK_MD0280_BLACK);
}
