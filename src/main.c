/**
 * ============================================================
 *  游戏机项目 — STM32F103C8T6 (实际CBT6, 128K Flash)
 * ============================================================
 *
 * 接线方案:
 *   LCD (ATK-MD0280 2.8寸 TFT, ILI9341, 8080 16位并口):
 *     D0-D15 : PB0-PB15 (PB2缺位, D2用PC13代替)
 *     RS=PA8  WR=PA9  RD=PA10  CS=PA11  RST=PA12
 *     BL=PA15  VDD=3.3V  LEDA=5V  GND=GND
 *
 *   触摸 (XPT2046 SPI, 未启用):
 *     CLK=PA4  MISO=PA5  MOSI=PA6  CS=PC14  PEN=PA3
 *
 *   摇杆 (KY-023 双轴模拟):
 *     VRx=PA0(ADC1_CH0)  VRy=PA1(ADC1_CH1)  SW=PA2
 *
 *   蜂鸣器 (无源, TIM3_CH2 PWM):
 *     IO=PA7
 *
 *   调试: SWDIO=PA13  SWCLK=PA14
 *
 *   烧录: ST-LINK_CLI -c SWD UR -ME -P firmware.bin 0x08000000 -V -Rst
 *   注意: BOOT0必须接GND
 * ============================================================
 */
#include "stm32f1xx_hal.h"
#include "sys.h"
#include "delay.h"
#include "atk_md0280.h"
#include "joystick.h"
#include "gfx.h"
#include "game.h"
#include "menu.h"
#include "history.h"
#include "buzzer.h"

static GameState game;

int main(void)
{
    int restart=0;
    uint16_t joy_x,joy_y;

    HAL_Init(); sys_stm32_clock_init(RCC_PLL_MUL9); delay_init(72);
    atk_md0280_init(); joystick_init(); buzzer_init();

    while(1)
    {
        if(!restart) menu_show_start();
        restart=1;

        int diff=menu_show_difficulty();  /* 0=简单 1=困难 */
        game_init(&game, diff);
        atk_md0280_clear(ATK_MD0280_BLACK);

        while(game.running)
        {
            joystick_read(&joy_x,&joy_y);
            game_update(&game,joy_x,joy_y);
            {
                int ht=game_hit_test(&game);
                if(ht==1){
                    buzzer_beep(400,100); delay_ms(50);
                    buzzer_beep(400,100); delay_ms(50);
                    atk_md0280_fill(0,0,239,319,ATK_MD0280_RED);
                    delay_ms(300);
                    atk_md0280_fill(0,0,239,319,ATK_MD0280_BLACK);
                    if(game.lives==0) game.running=0;
                }
                if(ht==2) buzzer_beep(800,30);   /* 金币嘀 */
                if(ht==3) buzzer_beep(600,40);   /* 护盾嘟 */
                if(ht==4) buzzer_beep(300,60);   /* 中毒嗡 */
            }
            game_draw(&game);
            game_draw_hud(game.lives, game.score, game.time_left, game.combo, game.shield, game.poison);
            if(game.time_left==0) game.running=0;
            delay_ms(20);
        }

        history_add(game.score, game.difficulty);
        atk_md0280_clear(ATK_MD0280_BLACK);
        if(menu_show_gameover(game.score)==0) restart=1;
        else restart=0;
    }
}
