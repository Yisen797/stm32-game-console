/**
 * KY-023 双轴摇杆驱动 — 单通道依次转换
 */

#include "joystick.h"

ADC_HandleTypeDef hadc1;

void joystick_init(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_ADC1_CLK_ENABLE();

    /* PA0, PA1 → 模拟输入 */
    gpio.Pin  = GPIO_PIN_0 | GPIO_PIN_1;
    gpio.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &gpio);

    /* PA2 → 按键输入 */
    gpio.Pin  = GPIO_PIN_2;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &gpio);

    /* ADC1 基础配置 (通道在读取时切换) */
    hadc1.Instance                   = ADC1;
    hadc1.Init.ScanConvMode          = DISABLE;          /* 单通道 */
    hadc1.Init.ContinuousConvMode    = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion       = 1;
    HAL_ADC_Init(&hadc1);
}

static uint16_t adc_read_ch(uint32_t ch)
{
    ADC_ChannelConfTypeDef cfg = {0};
    cfg.Channel      = ch;
    cfg.Rank         = ADC_REGULAR_RANK_1;
    cfg.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc1, &cfg);

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    uint16_t val = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    return val;
}

void joystick_read(uint16_t *x, uint16_t *y)
{
    *x = adc_read_ch(ADC_CHANNEL_0);   /* PA0 = VRx */
    *y = adc_read_ch(ADC_CHANNEL_1);   /* PA1 = VRy */
}

uint8_t joystick_pressed(void)
{
    return (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET) ? 1 : 0;
}
