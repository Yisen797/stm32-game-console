/**
 * menu.c — 菜单界面实现
 */
#include <stdio.h>
#include "menu.h"
#include "joystick.h"
#include "atk_md0280.h"
#include "delay.h"
#include "history.h"
#include "gfx.h"
#include "buzzer.h"
#include "melody.h"

void menu_show_start(void)
{
    atk_md0280_set_disp_dir(ATK_MD0280_LCD_DISP_DIR_0);
    atk_md0280_clear(ATK_MD0280_BLACK);
    atk_md0280_set_disp_dir(ATK_MD0280_LCD_DISP_DIR_90);
    atk_md0280_fill(0,0,319,239,ATK_MD0280_BLACK);
    /* 粒子: 4色随机 */
    #define NP 50
    int16_t px[NP],py[NP]; uint16_t pc[NP]; uint8_t pa[NP];
    uint16_t cls[4]={ATK_MD0280_RED,ATK_MD0280_GREEN,ATK_MD0280_MAGENTA,ATK_MD0280_YELLOW};
    static uint32_t rng2=54321;
    for(int i=0;i<NP;i++) pa[i]=0;
    int af=0, pt=0, mi=0, mt=0;
    while(!joystick_pressed()){
        /* 诺基亚铃声循环 */
        if(mi>=MELODY_LEN){mi=0;}
        if(mt<=0){Note n=melody[mi];buzzer_beep_wave(n.freq);mt=n.ms/20;mi++;}
        if(mt>0)mt--;
        rng2=rng2*1103515245+12345;
        if((rng2&0x0F)==0) for(int i=0;i<NP;i++) if(!pa[i]){
            px[i]=319; py[i]=10+((rng2>>10)%220); pa[i]=1;
            pc[i]=cls[(rng2>>8)%4];
            break;
        }
        for(int i=0;i<NP;i++){
            if(!pa[i])continue;
            gfx_circle(px[i],py[i],2,ATK_MD0280_BLACK);
            px[i]-=2; if(px[i]<-5) pa[i]=0;
        }
        int icx=160,icy=100,iR=20;  /* 图标位置 */
        for(int i=0;i<NP;i++) if(pa[i]){
            int dx=px[i]-icx,dy=py[i]-icy;
            if(dx*dx+dy*dy > iR*iR)  /* 不在图标内才画 */
                gfx_circle(px[i],py[i],2,pc[i]);
        }
        /* 按钮+文字 (覆盖粒子) */
        atk_md0280_draw_rect(100,170,220,230,ATK_MD0280_WHITE);
        atk_md0280_show_string(140,182,40,36,"START",ATK_MD0280_LCD_FONT_16,ATK_MD0280_WHITE);
        /* 吃豆人图标 */
        int cx=160,cy=100,R=20,mh=(af/16%2)?3:13;
        gfx_circle(cx,cy,R,ATK_MD0280_YELLOW);
        for(int dx=0;dx<=R;dx++){int dym=mh*dx/R;for(int dy=-dym;dy<=dym;dy++)atk_md0280_draw_point(cx+dx,cy+dy,ATK_MD0280_BLACK);}
        atk_md0280_fill(cx+7,cy-9,cx+10,cy-7,ATK_MD0280_BLACK);
        af++; delay_ms(20);
    }
    buzzer_beep(800,60);
    delay_ms(300);
    atk_md0280_set_disp_dir(ATK_MD0280_LCD_DISP_DIR_0);
}

int menu_show_difficulty(void)
{
    int sel=0,lsel=-1;
    uint16_t jx,jy;
    atk_md0280_set_disp_dir(ATK_MD0280_LCD_DISP_DIR_90);
    atk_md0280_fill(0,0,319,239,ATK_MD0280_BLACK);
    atk_md0280_show_string(50,30,220,32,"SELECT MODE",ATK_MD0280_LCD_FONT_16,ATK_MD0280_WHITE);
    while(1){
        joystick_read(&jx,&jy);
        if(jy<1500&&sel<1){sel++;delay_ms(120);}
        if(jy>2500&&sel>0){sel--;delay_ms(120);}
        if(sel!=lsel){
            atk_md0280_fill(50,120,190,190,ATK_MD0280_BLACK);
            if(sel==0){
                atk_md0280_fill(50,120,190,155,ATK_MD0280_YELLOW);
                atk_md0280_show_string(68,128,66,20,"EASY",ATK_MD0280_LCD_FONT_16,ATK_MD0280_BLACK);
                atk_md0280_draw_rect(50,165,190,190,ATK_MD0280_GRAY);
                atk_md0280_show_string(68,172,66,20,"HARD",ATK_MD0280_LCD_FONT_16,ATK_MD0280_GRAY);
            }else{
                atk_md0280_draw_rect(50,120,190,155,ATK_MD0280_GRAY);
                atk_md0280_show_string(68,128,66,20,"EASY",ATK_MD0280_LCD_FONT_16,ATK_MD0280_GRAY);
                atk_md0280_fill(50,165,190,190,ATK_MD0280_RED);
                atk_md0280_show_string(68,172,66,20,"HARD",ATK_MD0280_LCD_FONT_16,ATK_MD0280_BLACK);
            }
            lsel=sel;
        }
        if(joystick_pressed()){
            buzzer_beep(800,60); delay_ms(200);break;
        }
        delay_ms(50);
    }
    atk_md0280_set_disp_dir(ATK_MD0280_LCD_DISP_DIR_0);
    return sel;
}

int menu_show_gameover(int score)
{
    int sel=0,lsel=-1;
    uint16_t jx,jy;
    atk_md0280_set_disp_dir(ATK_MD0280_LCD_DISP_DIR_90);
    atk_md0280_fill(0,0,319,239,ATK_MD0280_BLACK);
    atk_md0280_show_string(70,40,200,32,"GAME OVER",ATK_MD0280_LCD_FONT_16,ATK_MD0280_RED);
    char sb[16]; sprintf(sb,"SCORE:%d",score);
    atk_md0280_show_string(70,80,180,20,sb,ATK_MD0280_LCD_FONT_16,ATK_MD0280_YELLOW);
    /* 历史记录 */
    for(int i=0;i<5;i++){
        int hs=history_get_score(i);
        if(hs>=0){
            int d=history_get_diff(i);
            char hb[12]; sprintf(hb,"#%d %d",i+1,hs);
            uint16_t hc=(d==1)?ATK_MD0280_RED:0x8410;
            atk_md0280_show_string(70,100+i*12,80,12,hb,ATK_MD0280_LCD_FONT_12,hc);
        }
    }
    while(1){
        joystick_read(&jx,&jy);
        if(jy<1500&&sel<1){sel++;delay_ms(120);}
        if(jy>2500&&sel>0){sel--;delay_ms(120);}
        if(sel!=lsel){
            atk_md0280_fill(50,170,190,240,ATK_MD0280_BLACK);
            if(sel==0){
                atk_md0280_fill(50,170,190,200,ATK_MD0280_WHITE);
                atk_md0280_show_string(62,175,76,20,"RESTART",ATK_MD0280_LCD_FONT_16,ATK_MD0280_BLACK);
                atk_md0280_draw_rect(50,210,190,230,ATK_MD0280_GRAY);
                atk_md0280_show_string(68,215,64,20,"HOME",ATK_MD0280_LCD_FONT_16,ATK_MD0280_GRAY);
            }else{
                atk_md0280_draw_rect(50,170,190,200,ATK_MD0280_GRAY);
                atk_md0280_show_string(62,175,76,20,"RESTART",ATK_MD0280_LCD_FONT_16,ATK_MD0280_GRAY);
                atk_md0280_fill(50,210,190,230,ATK_MD0280_WHITE);
                atk_md0280_show_string(68,215,64,20,"HOME",ATK_MD0280_LCD_FONT_16,ATK_MD0280_BLACK);
            }
            lsel=sel;
        }
        if(joystick_pressed()){buzzer_beep(800,60);delay_ms(200);break;}
        delay_ms(50);
    }
    atk_md0280_set_disp_dir(ATK_MD0280_LCD_DISP_DIR_0);
    return sel;
}
