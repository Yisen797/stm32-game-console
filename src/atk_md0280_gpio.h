/**
 * ATK-MD0280 GPIO 引脚配置 (适配 F103C8T6 最小系统板)
 *
 * LCD 8080 16位并口:
 *   D0-D15: PB0-PB15 (PB2 缺位, D2 移至 PC14)
 *   控制线: RS=PA8  WR=PA9  RD=PA10  CS=PA11  RST=PA12
 *   背光:   BL=PC13
 */

#ifndef __ATK_MD0280_GPIO_H
#define __ATK_MD0280_GPIO_H

#include "stm32f1xx_hal.h"

/* ================================================================
 * 控制引脚
 * ================================================================ */
#define ATK_MD0280_GPIO_RS_PORT            GPIOA
#define ATK_MD0280_GPIO_RS_PIN             GPIO_PIN_8

#define ATK_MD0280_GPIO_WR_PORT            GPIOA
#define ATK_MD0280_GPIO_WR_PIN             GPIO_PIN_9

#define ATK_MD0280_GPIO_RD_PORT            GPIOA
#define ATK_MD0280_GPIO_RD_PIN             GPIO_PIN_10

#define ATK_MD0280_GPIO_CS_PORT            GPIOA
#define ATK_MD0280_GPIO_CS_PIN             GPIO_PIN_11

#define ATK_MD0280_GPIO_RST_PORT           GPIOA
#define ATK_MD0280_GPIO_RST_PIN            GPIO_PIN_12

/* ================================================================
 * 数据总线: PB0-PB15 (PB2=BOOT1 缺位, D2 用 PC14 补)
 *
 * GPIOB bit 映射:
 *   PB0=D0, PB1=D1, (PB2=缺), PB3=D3, PB4=D4, ... PB15=D15
 * GPIOC bit 14 = D2
 * ================================================================ */
#define ATK_MD0280_GPIO_DATA_PORT           GPIOB
#define ATK_MD0280_GPIO_DATA_PIN_ALL        0xFFFB   /* PB 除 bit2 外全部 */

#define ATK_MD0280_GPIO_D2_PORT            GPIOC
#define ATK_MD0280_GPIO_D2_PIN             GPIO_PIN_13    /* PC13=板载LED, 写数据时会闪=可视调试 */

/* ================================================================
 * IO 操作宏
 * ================================================================ */

/* 控制线 — 直接寄存器操作 (高速) */
#define ATK_MD0280_GPIO_RS(x)   do{ if(x) ATK_MD0280_GPIO_RS_PORT->BSRR = ATK_MD0280_GPIO_RS_PIN; \
                                      else  ATK_MD0280_GPIO_RS_PORT->BRR  = ATK_MD0280_GPIO_RS_PIN; }while(0)

#define ATK_MD0280_GPIO_WR(x)   do{ if(x) ATK_MD0280_GPIO_WR_PORT->BSRR = ATK_MD0280_GPIO_WR_PIN; \
                                      else  ATK_MD0280_GPIO_WR_PORT->BRR  = ATK_MD0280_GPIO_WR_PIN; }while(0)

#define ATK_MD0280_GPIO_RD(x)   do{ if(x) ATK_MD0280_GPIO_RD_PORT->BSRR = ATK_MD0280_GPIO_RD_PIN; \
                                      else  ATK_MD0280_GPIO_RD_PORT->BRR  = ATK_MD0280_GPIO_RD_PIN; }while(0)

#define ATK_MD0280_GPIO_CS(x)   do{ if(x) ATK_MD0280_GPIO_CS_PORT->BSRR = ATK_MD0280_GPIO_CS_PIN; \
                                      else  ATK_MD0280_GPIO_CS_PORT->BRR  = ATK_MD0280_GPIO_CS_PIN; }while(0)

#define ATK_MD0280_GPIO_RST(x)  do{ if(x) ATK_MD0280_GPIO_RST_PORT->BSRR = ATK_MD0280_GPIO_RST_PIN; \
                                      else  ATK_MD0280_GPIO_RST_PORT->BRR  = ATK_MD0280_GPIO_RST_PIN; }while(0)

/* 数据总线 — 16位一次写入 (D2 独立引脚) */
#define ATK_MD0280_GPIO_DATA_OUT(x)    do { \
    ATK_MD0280_GPIO_DATA_PORT->ODR = ((x) & 0xFFFB);             /* PB 写 15 位 */ \
    if ((x) & 0x0004) ATK_MD0280_GPIO_D2_PORT->BSRR = ATK_MD0280_GPIO_D2_PIN; \
    else              ATK_MD0280_GPIO_D2_PORT->BRR  = ATK_MD0280_GPIO_D2_PIN; \
} while(0)

/* 数据总线 — 读取 (需先切换为输入模式) */
#define ATK_MD0280_GPIO_READ_DATA()     ( \
    (ATK_MD0280_GPIO_DATA_PORT->IDR & 0xFFFB) |                   /* 读 PB (掩掉 bit2) */ \
    ((ATK_MD0280_GPIO_D2_PORT->IDR & ATK_MD0280_GPIO_D2_PIN) ? 0x0004 : 0x0000) \
)

/* ================================================================
 * 函数声明
 * ================================================================ */
void atk_md0280_gpio_init(void);
void atk_md0280_gpio_write_cmd(uint16_t cmd);
void atk_md0280_gpio_write_dat(uint16_t dat);
void atk_md0280_gpio_write_reg(uint16_t reg, uint16_t dat);
uint16_t atk_md0280_gpio_read_dat(void);

#endif
