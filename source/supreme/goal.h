#ifndef GOAL_H
#define GOAL_H

#include "mgldraw.h"
#include "guy.h"

void PrintGoalInfo(int x,int y,byte goal);
void GoalKilledSomebody(Guy *g,byte type,byte frozen);
TASK(void) ShowGoalEarned(byte num);
TASK(void) ShowBonusGoalEarned(char* title, char* text);
void GoalTallyPage(int score,byte perfect,byte destruct,int time);
void GoalTimeDist(void);
void GoalPurchase(void);
void GoalWinLevel(void);
void GoalFire(void);

void CompleteGoal(byte goal);

#endif
