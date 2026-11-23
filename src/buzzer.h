/**
 * buzzer.h — 无源蜂鸣器 PWM 驱动
 */
#ifndef __BUZZER_H
#define __BUZZER_H

void buzzer_init(void);
void buzzer_beep(int freq, int ms);
void buzzer_beep_wave(int freq);  /* 仅设置频率 */
void buzzer_stop(void);

#endif
