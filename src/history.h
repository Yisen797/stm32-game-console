/**
 * history.h — 积分记录 (带难度)
 */
#ifndef __HISTORY_H
#define __HISTORY_H

#define HIST_MAX 5

void history_add(int score, int difficulty);
int  history_get_score(int idx);
int  history_get_diff(int idx);  /* 0=简单 1=困难 */

#endif
