#ifndef GAMAMANAGER_H
#define GAMEMANAGER_H

#include "globals.h"


void initGame();
void getOnUpdate(u_char board[24][14]); 
void getOnPlaced(u_char nTetri[4][4], u_char holdTetri[4][4], int *hold);
void getOnClear(int *level, int *score);
int update(int input);

#endif