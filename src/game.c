/**
 * game.c — 统一物品系统 + 旋律播放
 */
#include <stdio.h>
#include "game.h"
#include "gfx.h"

static uint32_t rng=12345;
static uint32_t randn(void){rng=rng*1103515245+12345;return rng>>16;}

static int hit(int16_t x1,int16_t y1,uint8_t r1,int16_t x2,int16_t y2,uint8_t r2){
    int32_t dx=x1-x2,dy=y1-y2;
    return (dx*dx+dy*dy)<(int32_t)(r1+r2)*(r1+r2);
}

void game_init(GameState *g, int difficulty)
{
    g->px=120; g->py=200; g->lx=120; g->ly=200;
    g->mouth=1; g->last_mouth=-1;
    g->lives=3; g->score=0; g->time_left=60; g->combo=0; g->difficulty=difficulty;
    g->shield=0; g->shield_timer=180; g->poison=0; g->poison_timer=250;
    g->running=1; g->frame=0;
    for(int i=0;i<MAX_ITEM;i++) g->items[i].active=0;
}

void game_update(GameState *g, uint16_t jx, uint16_t jy)
{
    int ps=(g->poison>0)?2:4;
    if(jx<1800&&g->px<215-FW) g->px+=ps;
    else if(jx>2200&&g->px>FW) g->px-=ps;
    if(jy<1800&&g->py<319-FW) g->py+=ps;
    else if(jy>2200&&g->py>FW) g->py-=ps;

    g->mouth=((g->frame/15)%2)?0:2;

    /* 随机生成: 倒计时制 */
    static int obs_timer=20, coin_timer=50;
    int obs_min=(g->difficulty==0)?15:8;
    int obs_max=(g->difficulty==0)?35:20;
    int coin_min=(g->difficulty==0)?40:25;
    int coin_max=(g->difficulty==0)?60:35;

    if(--obs_timer<=0){
        obs_timer=obs_min+(randn()%(obs_max-obs_min+1));
        int n=1+(randn()%3);
        int sx=10+(randn()%180);
        for(int c=0;c<n;c++){
            for(int i=0;i<MAX_ITEM;i++){
                if(!g->items[i].active){
                    int dx=sx+c*(20+(randn()%25));
                    if(dx>200) dx=200;
                    g->items[i].x=dx; g->items[i].y=319+(randn()%25);
                    g->items[i].r=3; g->items[i].type=0; g->items[i].active=1;
                    g->items[i].lx=g->items[i].x; g->items[i].ly=g->items[i].y;
                    g->items[i].base_x=g->items[i].x;
                    g->items[i].just_spawned=1; break;
                }
            }
        }
    }
    if(--coin_timer<=0){
        coin_timer=coin_min+(randn()%(coin_max-coin_min+1));
        for(int i=0;i<MAX_ITEM;i++){
            if(!g->items[i].active){
                int cx=15+(randn()%195); if(cx>200)cx=200;
                g->items[i].x=cx; g->items[i].y=319+(randn()%20);
                g->items[i].r=4; g->items[i].type=1; g->items[i].active=1;
                g->items[i].lx=g->items[i].x; g->items[i].ly=g->items[i].y;
                g->items[i].base_x=g->items[i].x;
                g->items[i].just_spawned=1; break;
            }
        }
    }
    if(--g->shield_timer<=0){
        g->shield_timer=150+(randn()%200);
        for(int i=0;i<MAX_ITEM;i++){
            if(!g->items[i].active){
                int sx2=15+(randn()%195); if(sx2>200)sx2=200;
                g->items[i].x=sx2; g->items[i].y=319;
                g->items[i].r=5; g->items[i].type=2; g->items[i].active=1;
                g->items[i].lx=g->items[i].x; g->items[i].ly=g->items[i].y;
                g->items[i].base_x=g->items[i].x;
                g->items[i].just_spawned=1; break;
            }
        }
    }
    if(--g->poison_timer<=0){
        g->poison_timer=200+(randn()%200);
        for(int i=0;i<MAX_ITEM;i++){
            if(!g->items[i].active){
                int sx3=15+(randn()%195); if(sx3>200)sx3=200;
                g->items[i].x=sx3; g->items[i].y=319;
                g->items[i].r=5; g->items[i].type=3; g->items[i].active=1;
                g->items[i].lx=g->items[i].x; g->items[i].ly=g->items[i].y;
                g->items[i].base_x=g->items[i].x;
                g->items[i].just_spawned=1; break;
            }
        }
    }
    /* 移动 */
    for(int i=0;i<MAX_ITEM;i++){
        if(!g->items[i].active)continue;
        if(!g->items[i].just_spawned){g->items[i].lx=g->items[i].x; g->items[i].ly=g->items[i].y;}
        g->items[i].y-=4+(60-g->time_left)/12;
        if(g->items[i].y<-50){gfx_circle(g->items[i].lx,g->items[i].ly,g->items[i].r,ATK_MD0280_BLACK);g->items[i].active=0;}
    }

    if(g->frame%50==0&&g->time_left>0) g->time_left--;
    if(g->shield>0) g->shield--;
    if(g->poison>0) g->poison--;
    g->frame++;
}

int game_hit_test(GameState *g)
{
    for(int i=0;i<MAX_ITEM;i++){
        if(!g->items[i].active)continue;
        if(hit(g->px,g->py,FW,g->items[i].x,g->items[i].y,g->items[i].r)){
            int16_t ex=g->items[i].just_spawned?g->items[i].x:g->items[i].lx;
            int16_t ey=g->items[i].just_spawned?g->items[i].y:g->items[i].ly;
            gfx_circle(ex,ey,g->items[i].r,ATK_MD0280_BLACK); g->items[i].active=0;
            if(g->items[i].type==3){g->poison=250;g->shield=0;return 4;}
            if(g->items[i].type==2){if(g->poison>0)return 0;g->shield=250;return 3;}
            if(g->items[i].type==1){if(g->poison>0)return 0;g->score+=10;return 2;}
            if(g->items[i].type==0){if(g->shield>0){g->score+=5;return 0;}g->lives--;return 1;}
        }
    }
    return 0;
}

void game_draw(GameState *g)
{
    for(int i=0;i<MAX_ITEM;i++)
        if(g->items[i].active&&!g->items[i].just_spawned&&(g->items[i].x!=g->items[i].lx||g->items[i].y!=g->items[i].ly))
            gfx_circle(g->items[i].lx,g->items[i].ly,g->items[i].r,ATK_MD0280_BLACK);
    if(g->px!=g->lx||g->py!=g->ly||g->mouth!=g->last_mouth){
        gfx_circle(g->lx,g->ly,FW,ATK_MD0280_BLACK);
        g->lx=g->px; g->ly=g->py; g->last_mouth=g->mouth;
    }
    for(int i=0;i<MAX_ITEM;i++)
        if(g->items[i].active){
            uint16_t c=(g->items[i].type==3)?ATK_MD0280_GREEN:(g->items[i].type==2)?ATK_MD0280_MAGENTA:(g->items[i].type==1)?ATK_MD0280_YELLOW:ATK_MD0280_RED;
            gfx_circle(g->items[i].x,g->items[i].y,g->items[i].r,c); g->items[i].just_spawned=0;
        }
    uint16_t pc=(g->poison>0)?ATK_MD0280_GREEN:(g->shield>0)?ATK_MD0280_MAGENTA:ATK_MD0280_YELLOW;
    gfx_pacman(g->px,g->py,g->mouth,pc);
}

void game_draw_hud(int lives, int score, int time_left, int combo, int shield, int poison)
{
    atk_md0280_fill(216,0,239,319,0x4208);
    for(int i=0;i<3;i++) gfx_circle(228,15+i*22,3,i<lives?ATK_MD0280_RED:0x4208);
    char sb[8]; sprintf(sb,"%d",score);
    atk_md0280_fill(218,80,237,94,0x4208); atk_md0280_show_string(219,82,18,12,sb,ATK_MD0280_LCD_FONT_12,ATK_MD0280_YELLOW);
    char tb[4]; sprintf(tb,"%d",time_left);
    uint16_t tc=time_left>10?ATK_MD0280_WHITE:ATK_MD0280_RED;
    atk_md0280_fill(218,100,237,114,0x4208); atk_md0280_show_string(219,102,18,12,tb,ATK_MD0280_LCD_FONT_12,tc);
    if(shield>0){char sb2[6];sprintf(sb2,"S%d",shield/50);atk_md0280_fill(218,116,237,128,0x4208);atk_md0280_show_string(220,118,16,12,sb2,ATK_MD0280_LCD_FONT_12,ATK_MD0280_MAGENTA);}
    if(poison>0){char pb[6];sprintf(pb,"P%d",poison/50);atk_md0280_fill(218,130,237,142,0x4208);atk_md0280_show_string(220,132,16,12,pb,ATK_MD0280_LCD_FONT_12,ATK_MD0280_GREEN);}
    (void)combo;
}
