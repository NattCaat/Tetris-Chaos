#ifndef INTERFACE_H
#define INTERFACE_H

#include "globals.h"


// Defined in "src/interface.c"

void initNcurses();
void endNcurses();
void initInterface();
void destroyInterface();
void wait();
int getInput();
void showOnUpdate(u_char board[24][14]);
void showOnPlaced(u_char nTetri[4][4], u_char holdTetri[4][4], int isHeld);
void showOnClear(int level, int score);


// Defined in "src/menus.c"

void mainMenu();

#endif