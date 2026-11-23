/**
 * ATK-MD0280 GPIO 接口实现 (适配 F103C8T6)
 *
 * 写数据: GPIOB(15位) + PC14(补D2)
 * 读数据: 先切输入模式再读
 */

#include "atk_md0280_gpio.h"
#include "delay.h"

/**
 * GPIO 初始化: 控制线 + 数据线 + LCD 硬件复位
 */
void atk_md0280_gpio_init(void)
{
    GPIO_InitTypeDef gpio = {0};

    /* ---- 使能时钟 ---- */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    /* 释放 JTAG 调试引脚 (PB3/PB4/PA15 用作 GPIO) */
    __HAL_AFIO_REMAP_SWJ_NOJTAG();

    /* ---- 控制线: RS, WR, RD, CS, RST (PA8-PA12) ---- */
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;

    gpio.Pin = ATK_MD0280_GPIO_RS_PIN;
    HAL_GPIO_Init(ATK_MD0280_GPIO_RS_PORT, &gpio);

    gpio.Pin = ATK_MD0280_GPIO_WR_PIN;
    HAL_GPIO_Init(ATK_MD0280_GPIO_WR_PORT, &gpio);

    gpio.Pin = ATK_MD0280_GPIO_RD_PIN;
    HAL_GPIO_Init(ATK_MD0280_GPIO_RD_PORT, &gpio);

    gpio.Pin = ATK_MD0280_GPIO_CS_PIN;
    HAL_GPIO_Init(ATK_MD0280_GPIO_CS_PORT, &gpio);

    gpio.Pin = ATK_MD0280_GPIO_RST_PIN;
    HAL_GPIO_Init(ATK_MD0280_GPIO_RST_PORT, &gpio);

    /* ---- 数据线: PB0-PB15 (除 PB2) ---- */
    gpio.Pin = ATK_MD0280_GPIO_DATA_PIN_ALL;
    HAL_GPIO_Init(ATK_MD0280_GPIO_DATA_PORT, &gpio);

    /* ---- D2 补位: PC14 ---- */
    gpio.Pin = ATK_MD0280_GPIO_D2_PIN;
    HAL_GPIO_Init(ATK_MD0280_GPIO_D2_PORT, &gpio);

    /* ---- 控制线初始电平 ---- */
    ATK_MD0280_GPIO_RS(1);
    ATK_MD0280_GPIO_WR(1);
    ATK_MD0280_GPIO_RD(1);
    ATK_MD0280_GPIO_CS(1);

    /* ---- LCD 硬件复位 (RST 低 >10us, 再拉高等 120ms) ---- */
    ATK_MD0280_GPIO_RST(0);
    delay_ms(20);
    ATK_MD0280_GPIO_RST(1);
    delay_ms(150);
}

/**
 * 写命令 (RS=0)
 */
void atk_md0280_gpio_write_cmd(uint16_t cmd)
{
    ATK_MD0280_GPIO_RS(0);
    ATK_MD0280_GPIO_CS(0);
    ATK_MD0280_GPIO_DATA_OUT(cmd);
    ATK_MD0280_GPIO_WR(0);
    ATK_MD0280_GPIO_WR(1);
    ATK_MD0280_GPIO_CS(1);
}

/**
 * 写数据 (RS=1)
 */
void atk_md0280_gpio_write_dat(uint16_t dat)
{
    ATK_MD0280_GPIO_RS(1);
    ATK_MD0280_GPIO_CS(0);
    ATK_MD0280_GPIO_DATA_OUT(dat);
    ATK_MD0280_GPIO_WR(0);
    ATK_MD0280_GPIO_WR(1);
    ATK_MD0280_GPIO_CS(1);
}

/**
 * 写寄存器 = 写命令 + 写数据
 */
void atk_md0280_gpio_write_reg(uint16_t reg, uint16_t dat)
{
    atk_md0280_gpio_write_cmd(reg);
    atk_md0280_gpio_write_dat(dat);
}

/**
 * 读数据: 切换数据总线为输入 → 读取 → 切回输出
 */
uint16_t atk_md0280_gpio_read_dat(void)
{
    GPIO_InitTypeDef gpio = {0};
    uint16_t dat;

    /* 切换数据总线 (PB) 为输入 */
    gpio.Pin   = ATK_MD0280_GPIO_DATA_PIN_ALL;
    gpio.Mode  = GPIO_MODE_INPUT;
    gpio.Pull  = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0280_GPIO_DATA_PORT, &gpio);

    /* 切换 D2 (PC14) 为输入 */
    gpio.Pin  = ATK_MD0280_GPIO_D2_PIN;
    HAL_GPIO_Init(ATK_MD0280_GPIO_D2_PORT, &gpio);

    /* 读时序 */
    ATK_MD0280_GPIO_RS(1);
    ATK_MD0280_GPIO_CS(0);
    ATK_MD0280_GPIO_RD(0);
    __asm volatile ("nop");
    __asm volatile ("nop");
    dat = ATK_MD0280_GPIO_READ_DATA();
    ATK_MD0280_GPIO_RD(1);
    ATK_MD0280_GPIO_CS(1);

    /* 切回输出 */
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pin  = ATK_MD0280_GPIO_DATA_PIN_ALL;
    HAL_GPIO_Init(ATK_MD0280_GPIO_DATA_PORT, &gpio);

    gpio.Pin  = ATK_MD0280_GPIO_D2_PIN;
    HAL_GPIO_Init(ATK_MD0280_GPIO_D2_PORT, &gpio);

    return dat;
}
