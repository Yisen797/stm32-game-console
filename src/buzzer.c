/**
 * buzzer.c — 无源蜂鸣器 (TIM3_CH2, PA7)
 */
#include "buzzer.h"
#include "stm32f1xx_hal.h"
#include "delay.h"

static TIM_HandleTypeDef htim3;

void buzzer_init(void)
{
    GPIO_InitTypeDef gpio={0};
    TIM_OC_InitTypeDef oc={0};

    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* PA7 → TIM3_CH2 (AF推挽) */
    gpio.Pin=GPIO_PIN_7;
    gpio.Mode=GPIO_MODE_AF_PP;
    gpio.Speed=GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA,&gpio);

    /* TIM3: PCLK1=36MHz, 计数器72MHz? No, TIM3 on APB1=36MHz, x2=72MHz */
    htim3.Instance=TIM3;
    htim3.Init.Prescaler=72-1;     /* 72MHz/72=1MHz → 1us/tick */
    htim3.Init.CounterMode=TIM_COUNTERMODE_UP;
    htim3.Init.Period=1000-1;      /* 默认1kHz */
    htim3.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim3);

    oc.OCMode=TIM_OCMODE_PWM1;
    oc.Pulse=500;                  /* 50%占空比 */
    oc.OCPolarity=TIM_OCPOLARITY_HIGH;
    HAL_TIM_PWM_ConfigChannel(&htim3,&oc,TIM_CHANNEL_2);

    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);  /* 先静音 */
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
}

void buzzer_beep(int freq, int ms)
{
    if(freq<100) freq=100;
    uint32_t period=1000000/freq;   /* 1MHz/freq */
    __HAL_TIM_SET_AUTORELOAD(&htim3,period-1);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,period/2);  /* 50% */
    delay_ms(ms);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);  /* 停 */
}

void buzzer_beep_wave(int freq)
{
    if(freq<50) freq=50;
    uint32_t period=1000000/freq;
    __HAL_TIM_SET_AUTORELOAD(&htim3,period-1);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,period/2);
}

void buzzer_stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);
}
