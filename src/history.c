/**
 * history.c — 积分记录
 */
#include "history.h"

static int scores[HIST_MAX];
static int diffs[HIST_MAX];
static int count;

void history_add(int s, int d)
{
    for(int i=HIST_MAX-1;i>0;i--){scores[i]=scores[i-1];diffs[i]=diffs[i-1];}
    scores[0]=s; diffs[0]=d;
    if(count<HIST_MAX) count++;
}

int history_get_score(int idx)
{
    return (idx<count)?scores[idx]:-1;
}

int history_get_diff(int idx)
{
    return (idx<count)?diffs[idx]:0;
}
