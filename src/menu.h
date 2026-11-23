/**
 * menu.h — 菜单界面
 */
#ifndef __MENU_H
#define __MENU_H

void menu_show_start(void);
int  menu_show_difficulty(void);  /* 返回 0=简单, 1=困难 */
int  menu_show_gameover(int score);

#endif
