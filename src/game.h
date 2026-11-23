/**
 * game.h — 吃豆人游戏核心逻辑
 */
#ifndef __GAME_H
#define __GAME_H

#include "stm32f1xx_hal.h"

#define MAX_ITEM 24

/* 统一物品: type=0障碍物, type=1豆子(加分) */
typedef struct {
    int16_t x, y, lx, ly, base_x;
    uint8_t r, type, active, just_spawned;
} Item;

/* 游戏状态 */
typedef struct {
    int16_t px, py;
    int16_t lx, ly;
    int    mouth, last_mouth;
    int    lives, score, time_left, combo, shield, shield_timer, poison, poison_timer;
    int    running;
    uint32_t frame;
    Item   items[MAX_ITEM];
    int    difficulty, melo_idx, melo_tick;
} GameState;

void game_init(GameState *g, int difficulty);
void game_update(GameState *g, uint16_t joy_x, uint16_t joy_y);
int  game_hit_test(GameState *g);
void game_draw(GameState *g);
void game_draw_hud(int lives, int score, int time_left, int combo, int shield, int poison);

#endif
