# STM32 掌上游戏机

基于 **STM32F103C8T6（实际 CBT6，128 KB Flash）** 的掌上游戏机：2.8 寸 TFT、摇杆、蜂鸣器，包含贪吃蛇 / 赛车等小游戏与音乐播放。

## 硬件接线

**LCD（正点原子 ATK-MD0280，2.8 寸 TFT，ILI9341，8080 16 位并口）**

```
D0-D15 : PB0-PB15（PB2 缺位，D2 用 PC13 代替）
RS=PA8  WR=PA9  RD=PA10  CS=PA11  RST=PA12
BL=PA15  VDD=3.3V  LEDA=5V  GND=GND
```

**触摸（XPT2046 SPI，未启用）**

```
CLK=PA4  MISO=PA5  MOSI=PA6  CS=PC14  PEN=PA3
```

**摇杆（KY-023）**

```
VRx=PA0(ADC1_CH0)  VRy=PA1(ADC1_CH1)  SW=PA2
```

**蜂鸣器**：无源，TIM3_CH2 PWM，IO=PA7

**调试 / 烧录**：SWDIO=PA13，SWCLK=PA14；**BOOT0 必须接 GND**

## 目录

```
src/
  main.c            主循环与菜单
  game.c            游戏逻辑
  gfx.c             图形绘制
  atk_md0280*.c/h   LCD 驱动
  joystick.c/h      摇杆
  melody.c/h        音乐
  dance.c           动画
  buzzer.c/h        音效
  history.c/h       最高分记录
platformio.ini
```

## 编译与烧录

```bash
pio run
pio run -t upload     # ST-Link
```

烧录命令示例：

```
ST-LINK_CLI -c SWD UR -ME -P firmware.bin 0x08000000 -V -Rst
```
