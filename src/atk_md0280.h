/**
 * ATK-MD0280 2.8寸 TFT LCD 主驱动 (ILI9341, 320x240)
 *
 * 引脚总览 (F103C8T6):
 *   数据: PB0-PB15 (PB2→PC14)
 *   控制: RS=PA8 WR=PA9 RD=PA10 CS=PA11 RST=PA12
 *   背光: BL=PC13
 *   触摸: CLK=PA4 MISO=PA5 MOSI=PA6 CS=PA15 PEN=PA3
 */

#ifndef __ATK_MD0280_H
#define __ATK_MD0280_H

#include "stm32f1xx_hal.h"

/* ---- 分辨率 ---- */
#define ATK_MD0280_LCD_WIDTH            240
#define ATK_MD0280_LCD_HEIGHT           320

/* ---- 触摸开关 ---- */
#define ATK_MD0280_USING_TOUCH          0   /* 触摸先关, 排查问题 */

/* ---- 字体开关 ---- */
#define ATK_MD0280_FONT_12              1
#define ATK_MD0280_FONT_16              1
#define ATK_MD0280_FONT_24              0   /* 太大, 占Flash */
#define ATK_MD0280_FONT_32              0

/* ---- 背光引脚 ---- */
#define ATK_MD0280_BL_PORT              GPIOA
#define ATK_MD0280_BL_PIN               GPIO_PIN_15    /* 原PC13被板载LED占用, 换PA15 */

#define ATK_MD0280_BL(x)                HAL_GPIO_WritePin(ATK_MD0280_BL_PORT, ATK_MD0280_BL_PIN, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)

/* ---- 颜色 (RGB565) ---- */
#define ATK_MD0280_WHITE        0xFFFF
#define ATK_MD0280_BLACK        0x0000
#define ATK_MD0280_BLUE         0x001F
#define ATK_MD0280_RED          0xF800
#define ATK_MD0280_GREEN        0x07E0
#define ATK_MD0280_CYAN         0x7FFF
#define ATK_MD0280_MAGENTA      0xF81F
#define ATK_MD0280_YELLOW       0xFFE0
#define ATK_MD0280_GRAY         0x8430

/* ---- 方向枚举 ---- */
typedef enum {
    ATK_MD0280_LCD_SCAN_DIR_L2R_U2D = 0x0008,
    ATK_MD0280_LCD_SCAN_DIR_L2R_D2U = 0x0088,
    ATK_MD0280_LCD_SCAN_DIR_R2L_U2D = 0x0048,
    ATK_MD0280_LCD_SCAN_DIR_R2L_D2U = 0x00C8,
    ATK_MD0280_LCD_SCAN_DIR_U2D_L2R = 0x0028,
    ATK_MD0280_LCD_SCAN_DIR_U2D_R2L = 0x0068,
    ATK_MD0280_LCD_SCAN_DIR_D2U_L2R = 0x00A8,
    ATK_MD0280_LCD_SCAN_DIR_D2U_R2L = 0x00E8,
} atk_md0280_lcd_scan_dir_t;

typedef enum {
    ATK_MD0280_LCD_DISP_DIR_0 = 0,
    ATK_MD0280_LCD_DISP_DIR_90,
    ATK_MD0280_LCD_DISP_DIR_180,
    ATK_MD0280_LCD_DISP_DIR_270,
} atk_md0280_lcd_disp_dir_t;

typedef enum {
    ATK_MD0280_LCD_FONT_12,
    ATK_MD0280_LCD_FONT_16,
    ATK_MD0280_LCD_FONT_24,
    ATK_MD0280_LCD_FONT_32,
} atk_md0280_lcd_font_t;

typedef enum {
    ATK_MD0280_NUM_SHOW_NOZERO = 0,
    ATK_MD0280_NUM_SHOW_ZERO,
} atk_md0280_num_mode_t;

/* ---- 返回值 ---- */
#define ATK_MD0280_EOK          0
#define ATK_MD0280_ERROR        1
#define ATK_MD0280_EINVAL       2

/* ---- LCD API ---- */
uint8_t  atk_md0280_init(void);
uint16_t atk_md0280_get_lcd_width(void);
uint16_t atk_md0280_get_lcd_height(void);
void     atk_md0280_backlight_on(void);
void     atk_md0280_backlight_off(void);
void     atk_md0280_display_on(void);
void     atk_md0280_display_off(void);
uint8_t  atk_md0280_set_scan_dir(atk_md0280_lcd_scan_dir_t scan_dir);
uint8_t  atk_md0280_set_disp_dir(atk_md0280_lcd_disp_dir_t disp_dir);
atk_md0280_lcd_disp_dir_t atk_md0280_get_disp_dir(void);
void     atk_md0280_fill(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color);
void     atk_md0280_clear(uint16_t color);
void     atk_md0280_draw_point(uint16_t x, uint16_t y, uint16_t color);
uint16_t atk_md0280_read_point(uint16_t x, uint16_t y);
void     atk_md0280_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void     atk_md0280_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void     atk_md0280_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
void     atk_md0280_show_char(uint16_t x, uint16_t y, char ch, atk_md0280_lcd_font_t font, uint16_t color);
void     atk_md0280_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *str, atk_md0280_lcd_font_t font, uint16_t color);
void     atk_md0280_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, atk_md0280_lcd_font_t font, uint16_t color);
void     atk_md0280_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, atk_md0280_num_mode_t mode, atk_md0280_lcd_font_t font, uint16_t color);
void     atk_md0280_show_pic(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *pic);

/* ---- 触摸 API ---- */
#if (ATK_MD0280_USING_TOUCH != 0)
#include "atk_md0280_touch.h"
#endif

#endif
