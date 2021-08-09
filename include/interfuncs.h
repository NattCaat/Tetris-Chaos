#ifndef INTERFUNCS_H
#define INTERFUNCS_H

#include "globals.h"

#define I_TETRI 1
#define O_TETRI 2
#define J_TETRI 3
#define L_TETRI 4
#define S_TETRI 5
#define Z_TETRI 6
#define T_TETRI 7
#define BLOCKED 8


// Defined in "src/menus.c"

void clearWin(WINDOW *win);
int mvwdrawbox(WINDOW *win, int y, int x, char ch, int height, int width);
int mvwdrawdia(WINDOW *win, int y, int x, char ch, int height, int width);
void settingMenu();

#endif