/**
 * dance.c — 蔡徐坤运球舞绘制
 */
#include "dance.h"
#include "atk_md0280.h"
#include <math.h>

#define PI 3.1415926f
#define SKIN 0xFD79   /* 肤色 */
#define HAIR 0x18E3   /* 深棕发 */
#define SUIT 0x0000   /* 黑西装 */
#define BALL 0xEAC0   /* 篮球橙 */

/* 计算端点: cx+len*sin(a), cy-len*cos(a) */
static void limb_end(int16_t cx, int16_t cy, int16_t a, int16_t len,
                     int16_t *ox, int16_t *oy)
{
    float rad = a * PI / 180.0f;
    *ox = cx + (int16_t)(len * sinf(rad));
    *oy = cy - (int16_t)(len * cosf(rad));
}

static void draw_thick_line(int16_t x1,int16_t y1,int16_t x2,int16_t y2,
                            int16_t w,uint16_t c)
{
    for(int16_t i=-w/2;i<=w/2;i++){
        atk_md0280_draw_line(x1+i,y1,x2+i,y2,c);
    }
}

void dance_draw_frame(uint16_t cx, uint16_t cy, int f)
{
    DanceFrame df = dance_frames[f];
    int16_t nx,ny, sx,sy, ex,ey, bx,by;
    int L=22;  /* 肢体长 */

    /* === 腿 === */
    limb_end(cx,cy+10,df.ll_a,L,&nx,&ny);
    draw_thick_line(cx,cy+10,nx,ny,4,SUIT);
    limb_end(cx,cy+10,df.rl_a,L,&nx,&ny);
    draw_thick_line(cx,cy+10,nx,ny,4,SUIT);

    /* === 身体 === */
    int16_t bx1,bx2,by1,by2;
    limb_end(cx,cy-5,df.body_a,12,&bx1,&by1);
    draw_thick_line(cx,cy-5,bx1,by1,8,SUIT);
    /* 白衬衫 */
    atk_md0280_draw_line(cx-2,cy-5,cx+2,cy-5,ATK_MD0280_WHITE);

    /* === 手臂 === */
    limb_end(cx,cy-5,df.la_a,L,&sx,&sy);
    draw_thick_line(cx,cy-5,sx,sy,3,SKIN);
    limb_end(cx,cy-5,df.ra_a,L,&ex,&ey);
    draw_thick_line(cx,cy-5,ex,ey,3,SKIN);

    /* === 头部 === */
    atk_md0280_draw_circle(cx,cy-20,8,SKIN);
    /* 发胶头 */
    atk_md0280_fill(cx-7,cy-32,cx+7,cy-25,HAIR);
    atk_md0280_fill(cx-9,cy-30,cx-5,cy-27,HAIR);
    atk_md0280_fill(cx+5,cy-30,cx+9,cy-27,HAIR);
    /* 眼睛 */
    atk_md0280_draw_point(cx-3,cy-21,SUIT);
    atk_md0280_draw_point(cx+3,cy-21,SUIT);
    /* 嘴 */
    atk_md0280_draw_point(cx-1,cy-17,0xF800);

    /* === 篮球 === */
    bx=cx+df.ball_dx; by=cy+df.ball_dy;
    atk_md0280_draw_circle(bx,by,5,BALL);
    /* 篮球纹路 */
    atk_md0280_draw_line(bx-4,by,bx+4,by,SUIT);
    atk_md0280_draw_line(bx,by-4,bx,by+4,SUIT);
}
